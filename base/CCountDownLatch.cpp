//
// Created by hc on 2020/4/29.
//

#include "CCountDownLatch.h"
using namespace neco;

CCountDownLatch::CCountDownLatch(int nCount)
:m_clsMutex(),m_clsCondition(m_clsMutex),m_nCount(nCount)
{

}

void CCountDownLatch::Wait()
{
    CMutexLockGuard lockGuard(m_clsMutex);
    while (m_nCount > 0)
        m_clsCondition.Wait();
}

void CCountDownLatch::CountDown()
{
    CMutexLockGuard lockGuard(m_clsMutex);
    --m_nCount;
    if(m_nCount == 0)
        m_clsCondition.NotifyAll();
}

int CCountDownLatch::GetCount() const
{
    CMutexLockGuard lockGuard(m_clsMutex);
    return m_nCount;
}