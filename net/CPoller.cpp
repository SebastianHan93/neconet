//
// Created by hc on 2020/5/8.
//

#include "CPoller.h"
#include "CChannel.h"
#include "CEventLoop.h"
#include <poll.h>
#include <assert.h>
#include <algorithm>

using namespace neco;
using namespace neco::net;

CPoller::CPoller(CEventLoop * Eventloop):m_pOwnerLoop(Eventloop)
{

}

CPoller::~CPoller()
{

}

CTimestamp CPoller::Poll(int nTimeoutMs,CHANNEL_LIST * vActiveChannels)
{
    int nNumEvents = ::poll(&*m_vPollFds.begin(),m_vPollFds.size(),nTimeoutMs);
    CTimestamp iTimeNow(CTimestamp::GetNowTimestamp());
    if(nNumEvents>0)
    {
        printf("Event happended\n");
        __FillActiveChannels(nNumEvents,vActiveChannels);
    }
    else if(nNumEvents==0)
    {
        printf("not happended\n");
    }
    else
    {
        printf("error CPoller::Poll\n");
    }
    return iTimeNow;
}

void CPoller::UpdateChannel(CChannel* iChannel)
{
    AssertInLoopThread();
    printf("fd=%d events=%d\n",iChannel->GetFd(),iChannel->GetEvents());
    if(iChannel->GetIndex()<0)
    {
        assert(m_mChannelsMap.find(iChannel->GetFd())==m_mChannelsMap.end());
        struct pollfd pFd;
        pFd.fd = iChannel->GetFd();
        pFd.events = static_cast<short>(iChannel->GetEvents());
        pFd.revents = 0;
        m_vPollFds.push_back(pFd);
        int nIdx = static_cast<int>(m_vPollFds.size())-1;
        iChannel->SetIndex(nIdx);
        m_mChannelsMap[pFd.fd] = iChannel;
    }
    else
    {
        assert(m_mChannelsMap.find(iChannel->GetFd())!=m_mChannelsMap.end());
        assert(m_mChannelsMap[iChannel->GetFd()] == iChannel);
        int nIdx = iChannel->GetIndex();
        assert(0<=nIdx && nIdx<static_cast<int>(m_vPollFds.size()));
        struct pollfd & pFd = m_vPollFds[nIdx];
        assert(pFd.fd==iChannel->GetFd() || pFd.fd==-iChannel->GetFd()-1);
        pFd.events = static_cast<short >(iChannel->GetEvents());
        pFd.revents = 0;
        if(iChannel->IsNoneEvent())
            pFd.fd = -iChannel->GetFd()-1;

    }
}

void CPoller::AssertInLoopThread()
{
    m_pOwnerLoop->AssertInLoopThread();
}

void CPoller::__FillActiveChannels(int nNumEvents,CHANNEL_LIST * vActiveChannels) const
{
    for (POLL_FD_LIST::const_iterator pFd = m_vPollFds.begin();
        pFd!=m_vPollFds.end()&&nNumEvents>0;++pFd)
    {
        if(pFd->revents>0)
        {
            --nNumEvents;
            CHANNEL_MAP::const_iterator ch = m_mChannelsMap.find(pFd->fd);
            assert(ch!=m_mChannelsMap.end());
            CChannel * channel = ch->second;
            assert(channel->GetFd() == pFd->fd);
            channel->SetREvents(pFd->revents);
            vActiveChannels->push_back(channel);
        }
    }
}

void CPoller::RemoveChannel(CChannel * iChannel)
{
    AssertInLoopThread();
    printf("fd=%d\n",iChannel->GetFd());
    assert(m_mChannelsMap.find(iChannel->GetFd())!=m_mChannelsMap.end());
    assert(m_mChannelsMap[iChannel->GetFd()] == iChannel);
    assert(iChannel->IsNoneEvent());
    int nIdx = iChannel->GetIndex();
    assert(0<=nIdx && nIdx<static_cast<int>(m_vPollFds.size()));
    const struct pollfd & pFd = m_vPollFds[nIdx];
    assert(pFd.fd == -iChannel->GetFd() -1 && pFd.events == iChannel->GetEvents());
    size_t n = m_mChannelsMap.erase(iChannel->GetFd());
    assert(n==1);(void)n;
    if(implicit_cast<size_t >(nIdx) == m_vPollFds.size()-1)
    {
        m_vPollFds.pop_back();
    }
    else
    {
        int nChannelAtEnd = m_vPollFds.back().fd;
        std::iter_swap(m_vPollFds.begin()+nIdx,m_vPollFds.end()-1);
        if(nChannelAtEnd < 0)
        {
            nChannelAtEnd = -nChannelAtEnd-1;
        }
        m_mChannelsMap[nChannelAtEnd]->SetIndex(nIdx);
        m_vPollFds.pop_back();
    }
}


