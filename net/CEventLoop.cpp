//
// Created by hc on 2020/5/7.
//
#include <iostream>
#include <assert.h>
#include <poll.h>
#include <unistd.h>
#include <sys/eventfd.h>

#include "CEventLoop.h"
#include "../base/CurrentThread.h"
#include "CPoller.h"
#include "CChannel.h"
#include "CTimerQueue.h"

using namespace neco;
using namespace neco::net;
using std::cout;
using std::endl;

__thread CEventLoop* t_pLoopInThisThread = nullptr;
const int g_nKPollTimeMs = 10000;

static int CreateEventFd()
{
    int nEvtFd = ::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
    if(nEvtFd < 0)
    {
        std::cout << "Failed in eventfd" << std::endl;
        abort();
    }
    return nEvtFd;
}

CEventLoop* CEventLoop::GetEventLoopOfCurrentThread()
{
    return t_pLoopInThisThread;
}

CEventLoop::CEventLoop()
    :m_bInLooping(false),
    m_pidThreadId(CurrentThread::Tid()),
    m_bQuit(false),
    m_bCallingPendingFunctors(false),
    m_unpPoller(new CPoller(this)),
    m_unpTimerQueue(new CTimerQueue(this)),
    m_nWakeupFd(CreateEventFd()),
    m_unpWakeupChannel(new CChannel(this,m_nWakeupFd))
{
    cout<< "EventLoop created " << this << " in thread " << m_pidThreadId << endl;
    if(t_pLoopInThisThread)
    {
        cout << "Another EventLoop "<<t_pLoopInThisThread
             << " exists in this thread " << m_pidThreadId;
    }
    else
    {
        t_pLoopInThisThread = this;
    }
    m_unpWakeupChannel->SetReadCallback(std::bind(&CEventLoop::__HandleRead,this));
    m_unpWakeupChannel->SetEnableReading();
}

CEventLoop::~CEventLoop()
{
    assert(!m_bInLooping);
    ::close(m_nWakeupFd);
    t_pLoopInThisThread = nullptr;
}

void CEventLoop::StartLoop()
{
    assert(!m_bInLooping);
    AssertInLoopThread();
    m_bInLooping = true;
    m_bQuit = false;
    while (!m_bQuit)
    {
        m_vActiveChannels.clear();
        m_unpPoller->Poll(g_nKPollTimeMs,&m_vActiveChannels);
        for (CHANNEL_LIST::iterator it = m_vActiveChannels.begin();it != m_vActiveChannels.end(); it++)
        {

            (*it)->HandleEvent(CTimestamp::GetNowTimestamp());
        }
        __DoPendingFunctors();
    }
    std::cout << "CEventLoop " << this << "stop looping" << std::endl;
    m_bInLooping = false;
}

void CEventLoop::AssertInLoopThread()
{
    if(!IsInLoopThread())
    {
        __AbortNotInLoopThread();
    }
}

bool CEventLoop::IsInLoopThread() const
{
    return static_cast<bool>(m_pidThreadId == CurrentThread::Tid());
}

void CEventLoop::__AbortNotInLoopThread()
{
    cout << "EventLoop::abortNotInLoopThread - EventLoop " << this
              << " was created in threadId_ = " << m_pidThreadId
              << ", current thread id = " <<  CurrentThread::Tid()<<endl;
}

void CEventLoop::QuitLoop()
{
    m_bQuit = true;
    if(!IsInLoopThread())
    {
        Wakeup();
    }
}

void CEventLoop::RemoveChannel(CChannel * iChannel)
{
    assert(iChannel->GetOwnerLoop() == this);
    AssertInLoopThread();
    m_unpPoller->RemoveChannel(iChannel);
}

void CEventLoop::UpdateChannel(CChannel * iChannel)
{
    assert(iChannel->GetOwnerLoop() == this);
    AssertInLoopThread();
    m_unpPoller->UpdateChannel(iChannel);
}

CTimestamp CEventLoop::PollReturnTime() const
{
    return m_iPollReturnTime;
}

CTimerId CEventLoop::RunAt(CTimestamp iTime,TIMER_CALL_BACK cb)
{
    return m_unpTimerQueue->AddTimer(std::move(cb),iTime,0.0);
}

CTimerId CEventLoop::RunAfter(double dfDelay,TIMER_CALL_BACK cb)
{
    CTimestamp iTime(AddTime(CTimestamp::GetNowTimestamp(),dfDelay));
    return RunAt(iTime,std::move(cb));
}

CTimerId CEventLoop::RunEvery(double dfInterval,TIMER_CALL_BACK cb)
{
    CTimestamp iTime(AddTime(CTimestamp::GetNowTimestamp(),dfInterval));
    return m_unpTimerQueue->AddTimer(std::move(cb),iTime,dfInterval);

}

void CEventLoop::RunInLoop(FUNCTOR cb)
{
    if(IsInLoopThread())
    {
        cb();
    }
    else
    {
        QueueInLoop(std::move(cb));
    }
}

void CEventLoop::QueueInLoop(FUNCTOR cb)
{
    {
        CMutexLockGuard lock(m_iMutex);
        m_vPendingFunctors.push_back(std::move(cb));
    }
    if(!IsInLoopThread() || m_bCallingPendingFunctors)
    {
        Wakeup();
    }
}

void CEventLoop::Wakeup()
{
    uint64_t nOne = 1;
    ssize_t n = ::write(m_nWakeupFd,&nOne, sizeof(nOne));
    if(n!= sizeof(nOne))
    {
        std::cout << "CEventLoop::Wakeup() writes " << n << " bytes instead of 8" << std::endl;
    }
}

void CEventLoop::__HandleRead()
{
    uint64_t nOne = 1;
    ssize_t n = ::read(m_nWakeupFd,&nOne,sizeof(nOne));
    if(n!= sizeof(nOne))
    {
        std::cout << "CEventLoop::Wakeup() reads " << n << " bytes instead of 8" << std::endl;
    }
}

void CEventLoop::__DoPendingFunctors()
{
    std::vector<FUNCTOR> vFunctors;
    m_bCallingPendingFunctors = true;
    {
        CMutexLockGuard iLockGuard(m_iMutex);
        vFunctors.swap(m_vPendingFunctors);
    }

    for(size_t i = 0;i<vFunctors.size();++i)
    {
        vFunctors[i]();
    }
    m_bCallingPendingFunctors = false;
}