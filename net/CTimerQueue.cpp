//
// Created by hc on 2020/5/8.
//
#include <sys/timerfd.h>
#include <unistd.h>
#include "CTimerQueue.h"
#include "CTimer.h"
#include "CTimerId.h"
#include "CEventLoop.h"

#include <iostream>


namespace neco
{
    namespace detail
    {
        int CreateTimerFd()
        {
            int nTimerFd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
            if (nTimerFd<0)
            {
                std::cout<< "Failed in timerfd_create" << std::endl;
            }
            return nTimerFd;
        }

        void ReadTimerfd(int nTimerFd, CTimestamp iTimeNow)
        {
            uint64_t nHowMany;
            ssize_t n = ::read(nTimerFd,&nHowMany, sizeof(nHowMany));
            std::cout<<"TimerQueue::handleRead() " << nHowMany << " at " << iTimeNow.ToString()<<std::endl;
            if(n!=sizeof(nHowMany))
            {
                std::cout<<"TimerQueue::handleRead() reads "<<n<<" bytes instead of 8"<<std::endl;
            }
        }

        struct timespec HowMuchTimeFromNow(CTimestamp iWhen)
        {
            int64_t nMicroseconds = iWhen.GetMicroSecondsSinceEpoch() - CTimestamp::GetNowTimestamp().GetMicroSecondsSinceEpoch();
            if(nMicroseconds<100)
            {
                nMicroseconds = 100;
            }
            struct timespec iTs;
            iTs.tv_sec = static_cast<time_t >(nMicroseconds/CTimestamp::sm_nMicroSecondsPerSecond);
            iTs.tv_nsec = static_cast<long>((nMicroseconds%CTimestamp::sm_nMicroSecondsPerSecond)*1000);
            return iTs;
        }

        void ResetTimerFd(int nTimerFd,CTimestamp iExpiration)
        {
            struct itimerspec iNewValue;
            struct itimerspec iOldValue;
            bzero(&iNewValue,sizeof(iNewValue));
            bzero(&iOldValue,sizeof(iOldValue));
            iNewValue.it_value = HowMuchTimeFromNow(iExpiration);
            int nRet = ::timerfd_settime(nTimerFd,0,&iNewValue,&iOldValue);
            if(nRet)
            {
                std::cout << "timerfd_settime()";
            }
        }


    }
}

using namespace neco;
using namespace neco::net;
using namespace neco::detail;

CTimerQueue::CTimerQueue(CEventLoop * iEventLoop)
    :m_pEventLoop(iEventLoop),
     m_nTimerfd(CreateTimerFd()),
     m_iTimerFdChannel(iEventLoop,m_nTimerfd),
     m_ssTimers()
{
    m_iTimerFdChannel.SetReadCallback(std::bind(&CTimerQueue::__HandleRead,this));
    m_iTimerFdChannel.SetEnableReading();
}

CTimerQueue::~CTimerQueue()
{
    ::close(m_nTimerfd);
    for(TIMER_LIST::iterator it=m_ssTimers.begin();it!=m_ssTimers.end();++it)
    {
        delete it->second;
    }
}

CTimerId CTimerQueue::AddTimer(TIMER_CALL_BACK cb,CTimestamp iWhen, double dfInterval)
{
    CTimer * iTimer = new CTimer(std::move(cb),iWhen,dfInterval);
    m_pEventLoop->AssertInLoopThread();
    bool bEarliestChanged = __InsertTimer(iTimer);
    if(bEarliestChanged)
    {
        ResetTimerFd(m_nTimerfd,iTimer->GetExpiration());
    }
    return CTimerId(iTimer);
}

void CTimerQueue::__HandleRead()
{
    m_pEventLoop->AssertInLoopThread();
    CTimestamp iNow(CTimestamp::GetNowTimestamp());
    ReadTimerfd(m_nTimerfd,iNow);
    std::vector<ENTRY> vExpired = __GetExpired(iNow);
    for (std::vector<ENTRY>::iterator it = vExpired.begin();
        it!= vExpired.end(); ++it)
    {
          it->second->Run();
    }
    __ResetTimers(vExpired,iNow);
}

std::vector<CTimerQueue::ENTRY> CTimerQueue::__GetExpired(CTimestamp iNow)
{
    std::vector<ENTRY> vExpired;
    ENTRY spSentry = std::make_pair(iNow, reinterpret_cast<CTimer*>(UINTPTR_MAX));
    TIMER_LIST::iterator it = m_ssTimers.lower_bound(spSentry);
    assert(it == m_ssTimers.end() || iNow<it->first);
    std::copy(m_ssTimers.begin(),it,back_inserter(vExpired));
    m_ssTimers.erase(m_ssTimers.begin(),it);
    return vExpired;
}

void CTimerQueue::__ResetTimers(const std::vector<ENTRY> & vExpired,CTimestamp iNow)
{
    CTimestamp iNextExpire;
    for(std::vector<ENTRY>::const_iterator it = vExpired.begin();
        it!=vExpired.end();++it)
    {
        if(it->second->GetRepeat())
        {
            it->second->Restart(iNow);
            __InsertTimer(it->second);
        }
        else
        {
            delete it->second;
        }
    }
    if(!m_ssTimers.empty())
    {
        iNextExpire = m_ssTimers.begin()->second->GetExpiration();
    }
    if(iNextExpire.IsValid())
    {
        ResetTimerFd(m_nTimerfd,iNextExpire);
    }
}

bool CTimerQueue::__InsertTimer(CTimer * iTimer)
{
    bool bEarliestChanged = false;
    CTimestamp iWhen = iTimer->GetExpiration();
    TIMER_LIST::iterator it = m_ssTimers.begin();
    if(it == m_ssTimers.end() || iWhen<it->first)
    {
        bEarliestChanged = true;
    }
    std::pair<TIMER_LIST::iterator,bool> spResult = m_ssTimers.insert(std::make_pair(iWhen,iTimer));
    assert(spResult.second);
    return bEarliestChanged;
}