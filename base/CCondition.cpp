//
// Created by hc on 2020/4/29.
//

#include <stdint-gcc.h>
#include <cerrno>
#include "CCondition.h"
CCondition::CCondition(CMutexLock & rMutexLock):m_rMutex(rMutexLock)
{
    MCHECK(pthread_cond_init(&m_pctCond,NULL));
}

CCondition::~CCondition()
{
    MCHECK(pthread_cond_destroy(&m_pctCond));
}

void CCondition::Wait()
{
    CMutexLock::CUnassignGuard ug(m_rMutex);
    MCHECK(pthread_cond_wait(&m_pctCond,m_rMutex.GetPthreadMutex()));
}

bool CCondition::WaitForSeconds(double dSeconds)
{
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME,&abstime);
    const int64_t kNanoSecondsPerSecond = 1000000000;
    int64_t nanoSeconds = static_cast<int64_t >(dSeconds*kNanoSecondsPerSecond);
    abstime.tv_sec += static_cast<time_t >((abstime.tv_sec+nanoSeconds)/kNanoSecondsPerSecond);
    abstime.tv_nsec = static_cast<long>((abstime.tv_nsec+nanoSeconds)%kNanoSecondsPerSecond);
    CMutexLock::CUnassignGuard ug(m_rMutex);
    return ETIMEDOUT == pthread_cond_timedwait(&m_pctCond,m_rMutex.GetPthreadMutex(),&abstime);
}

void CCondition::Notify()
{
    MCHECK(pthread_cond_signal(&m_pctCond));
}

void CCondition::NotifyAll()
{
    MCHECK(pthread_cond_broadcast(&m_pctCond));
}
