//
// Created by hc on 2020/4/27.
//

#include "CThread.h"
#include "CThreadData.h"

namespace neco
{
    CThread::CThread(THREAD_FUNC pfnFunc, const string& sName)
    :m_bStarted(false),m_bJoined(false),m_pthreadId(0),
    m_tid(0),m_pfnFunc(std::move(pfnFunc)),m_sName(sName),m_clsLatch(1)
    {
        __SetDefaultName();
    }

    CThread::~CThread()
    {
        if(m_bStarted && !m_bJoined)
        {
            pthread_detach(m_pthreadId);
        }
    }

    void CThread::Start()
    {
        assert(!m_bStarted);
        m_bStarted = true;
        detail::CThreadData * data = new detail::CThreadData(m_pfnFunc,m_sName,&m_tid,&m_clsLatch);
        if(pthread_create(&m_pthreadId,NULL,&detail::StartThread,data))
        {
            m_bStarted = false;
            delete data;
        }
        else
        {
            m_clsLatch.Wait();
            assert(m_tid > 0);
        }
    }

    int CThread::ThreadJoin()
    {
        assert(m_bStarted);
        assert(!m_bJoined);
        m_bJoined = true;
        return pthread_join(m_pthreadId,NULL);
    }

    bool CThread::IsStarted() const
    {
        return m_bStarted;
    }

    pthread_t CThread::GetTid() const
    {
        return m_tid;
    }

    const string& CThread::GetName() const
    {
        return m_sName;
    }

    int CThread::NumCreated()
    {
        return s_aiNumCreated.Get();
    }

    void CThread::__SetDefaultName()
    {
        int num = s_aiNumCreated.IncrementAndGet();
        if(m_sName.empty())
        {
            char buf[32];
            snprintf(buf,sizeof buf,"Thread%d",num);
            m_sName = buf;
        }
    }
}

