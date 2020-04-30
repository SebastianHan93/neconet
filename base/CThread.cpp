//
// Created by hc on 2020/4/27.
//

#include "CThread.h"
namespace neco
{
    CThread::CThread(THREAD_FUNC pfnFunc, const string& sName)
    :m_bStarted(false),m_bJoined(false),m_pthreadId(0),
    m_tid(0),m_pfnFunc(std::move(pfnFunc)),m_sName(sName),m_clsLatch(1)
    {

    }

    CThread::~CThread()
    {

    }

    void Start()
    {

    }

    int ThreadJoin()
    {

    }

    bool Started() const
    {

    }

    pthread_t GetTid() const
    {

    }

    const string& GetName() const
    {

    }

    static int NumCreated()
    {

    }

    void __SetDefaultName()
    {

    }
}

