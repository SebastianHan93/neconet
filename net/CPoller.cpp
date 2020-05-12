//
// Created by hc on 2020/5/8.
//

#include "CPoller.h"
#include "CChannel.h"
#include "CEventLoop.h"
#include <poll.h>
#include <assert.h>

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

void CPoller::UpdateChannel(CChannel* channel)
{
    AssertInLoopThread();
    printf("fd=%d events=%d\n",channel->GetFd(),channel->GetEvents());
    if(channel->GetIndex()<0)
    {
        assert(m_mChannelsMap.find(channel->GetFd())==m_mChannelsMap.end());
        struct pollfd pFd;
        pFd.fd = channel->GetFd();
        pFd.events = static_cast<short>(channel->GetEvents());
        pFd.revents = 0;
        m_vPollFds.push_back(pFd);
        int nIdx = static_cast<int>(m_vPollFds.size())-1;
        channel->SetIndex(nIdx);
        m_mChannelsMap[pFd.fd] = channel;
    }
    else
    {
        assert(m_mChannelsMap.find(channel->GetFd())!=m_mChannelsMap.end());
        assert(m_mChannelsMap[channel->GetFd()] == channel);
        int nIdx = channel->GetIndex();
        assert(0<=nIdx && nIdx<static_cast<int>(m_vPollFds.size()));
        struct pollfd & pFd = m_vPollFds[nIdx];
        assert(pFd.fd==channel->GetFd() || pFd.fd==-1);
        pFd.events = static_cast<short >(channel->GetEvents());
        pFd.revents = 0;
        if(channel->IsNoneEvent())
            pFd.fd = -1;

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

