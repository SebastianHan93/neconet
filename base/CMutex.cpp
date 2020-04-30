//
// Created by hc on 2020/4/29.
//

#include "CMutex.h"
#include "CurrentThread.h"

CMutexLock::CMutexLock():m_pidHolder(0)
{
    MCHECK(pthread_mutex_init(&m_Mutex,NULL));
}

CMutexLock::~CMutexLock()
{
    assert(m_pidHolder == 0);
    MCHECK(pthread_mutex_destroy(&m_Mutex));
}

bool CMutexLock::IslockedByThisThread() const
{
    return m_pidHolder == CurrentThread::Tid();
}

void CMutexLock::AssertLocked() const ASSERT_CAPABILITY(this)
{
    assert(IslockedByThisThread());
}

void CMutexLock::Lock() ACQUIRE()
{
    MCHECK(pthread_mutex_lock(&m_Mutex));
    AssertLocked();
}

void CMutexLock::Unlock() RELEASE()
{
    UnassignHolder();
    MCHECK(pthread_mutex_unlock(&m_Mutex));
}

pthread_mutex_t * CMutexLock::GetPthreadMutex()
{
    return &m_Mutex;
}

void CMutexLock::UnassignHolder()
{
    m_pidHolder = 0;
}

void CMutexLock::AssignHolder()
{
    m_pidHolder = CurrentThread::Tid();
}

CMutexLock::CUnassignGuard::CUnassignGuard(CMutexLock& clsOwner):m_rOwner(clsOwner)
{
    m_rOwner.UnassignHolder();
}

CMutexLock::CUnassignGuard::~CUnassignGuard()
{
    m_rOwner.AssignHolder();
}

CMutexLockGuard::CMutexLockGuard(CMutexLock& cMutexLock) ACQUIRE(cMutexLock):m_rMutexlock(cMutexLock)
{
    m_rMutexlock.Lock();
}

CMutexLockGuard::~CMutexLockGuard() RELEASE()
{
    m_rMutexlock.Unlock();
}