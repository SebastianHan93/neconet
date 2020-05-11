//
// Created by hc on 2020/5/11.
//

#include "CEventLoopThread.h"
#include "CEventLoop.h"

using namespace neco;
using namespace neco::net;

CEventLoopThread::CEventLoopThread()
    :m_iEventLoop(nullptr),
     m_bExiting(false),
     m_iThread(std::bind(&CEventLoopThread::__ThreadFunc,this)),
     m_iMutex(),
     m_iCond(m_iMutex)
{

}

CEventLoopThread::~CEventLoopThread()
{
    m_bExiting = true;
    m_iEventLoop->QuitLoop();
    m_iThread.ThreadJoin();
}

CEventLoop * CEventLoopThread::StartLoop()
{
    assert(!m_iThread.IsStarted());
    m_iThread.Start();
    {
        CMutexLockGuard iLock(m_iMutex);
        while(m_iEventLoop == NULL)
        {
            m_iCond.Wait();
        }
    }
    return m_iEventLoop;
}

void CEventLoopThread::__ThreadFunc()
{
    CEventLoop iLoop;
    {
        CMutexLockGuard iLock(m_iMutex);
        m_iEventLoop = &iLoop;
        m_iCond.Notify();
    }
    iLoop.StartLoop();
}
