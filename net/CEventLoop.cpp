//
// Created by hc on 2020/5/7.
//
#include <iostream>
#include <assert.h>
#include <poll.h>
#include "CEventLoop.h"
#include "../base/CurrentThread.h"
#include "CPoller.h"
#include "CChannel.h"


using namespace neco;
using namespace neco::net;
using std::cout;
using std::endl;

__thread CEventLoop* t_pLoopInThisThread = nullptr;
const int g_nKPollTimeMs = 1000;

CEventLoop* CEventLoop::GetEventLoopOfCurrentThread()
{
    return t_pLoopInThisThread;
}


CEventLoop::CEventLoop()
    :m_bInLooping(false),
    m_pidThreadId(CurrentThread::Tid()),
    m_bQuit(false),
    m_unpPoller(new CPoller(this))

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
}

CEventLoop::~CEventLoop()
{
    assert(!m_bInLooping);
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
    }
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
}

void CEventLoop::UpdateChannel(CChannel * iChannel)
{
    assert(iChannel->GetOwnerLoop() == this);
    AssertInLoopThread();
    m_unpPoller->UpdateChannel(iChannel);
}