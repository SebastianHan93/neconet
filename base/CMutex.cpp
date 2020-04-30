//
// Created by hc on 2020/4/29.
//

#include "CMutex.h"
#include "CurrentThread.h"

neco::CMutexLock::CMutexLock():m_pidHolder(0)
{
    MCHECK(pthread_mutex_init(&m_Mutex,NULL));
}

neco::CMutexLock::~CMutexLock()
{
    assert(m_pidHolder == 0);
    MCHECK(pthread_mutex_destroy(&m_Mutex));
}

bool neco::CMutexLock::IslockedByThisThread() const
{
    return m_pidHolder == CurrentThread::Tid();
}

void neco::CMutexLock::AssertLocked() const ASSERT_CAPABILITY(this)
{
    assert(IslockedByThisThread());
}

void neco::CMutexLock::Lock() ACQUIRE()
{
    MCHECK(pthread_mutex_lock(&m_Mutex));
    AssertLocked();
}

void neco::CMutexLock::Unlock() RELEASE()
{
    UnassignHolder();
    MCHECK(pthread_mutex_unlock(&m_Mutex));
}

pthread_mutex_t * neco::CMutexLock::GetPthreadMutex()
{
    return &m_Mutex;
}

void neco::CMutexLock::UnassignHolder()
{
    m_pidHolder = 0;
}

void neco::CMutexLock::AssignHolder()
{
    m_pidHolder = CurrentThread::Tid();
}

neco::CMutexLock::CUnassignGuard::CUnassignGuard(CMutexLock& clsOwner):m_rOwner(clsOwner)
{
    m_rOwner.UnassignHolder();
}

neco::CMutexLock::CUnassignGuard::~CUnassignGuard()
{
    m_rOwner.AssignHolder();
}

neco::CMutexLockGuard::CMutexLockGuard(CMutexLock& cMutexLock) ACQUIRE(cMutexLock):m_rMutexlock(cMutexLock)
{
    m_rMutexlock.Lock();
}

neco::CMutexLockGuard::~CMutexLockGuard() RELEASE()
{
    m_rMutexlock.Unlock();
}