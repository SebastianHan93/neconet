//
// Created by hc on 2020/4/29.
//

#include "CCountDownLatch.h"
using namespace neco;

CCountDownLatch::CCountDownLatch(int iCount)
:m_clsMutex(),m_clsCondition(m_clsMutex),m_iCount(m_iCount)
{

}

void CCountDownLatch::Wait()
{
    CMutexLockGuard lockGuard(m_clsMutex);
    while (m_iCount > 0)
        m_clsCondition.Wait();
}

void CCountDownLatch::CountDown()
{
    CMutexLockGuard lockGuard(m_clsMutex);
    --m_iCount;
    if(m_iCount == 0)
        m_clsCondition.NotifyAll();
}

int CCountDownLatch::GetCount() const
{
    CMutexLockGuard lockGuard(m_clsMutex);
    return m_iCount;
}