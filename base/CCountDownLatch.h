//
// Created by hc on 2020/4/29.
//

#ifndef NECONET_CCOUNTDOWNLATCH_H
#define NECONET_CCOUNTDOWNLATCH_H


#include "noncopyable.h"
#include "CMutex.h"
#include "CCondition.h"

class CCountDownLatch : noncopyable{
public:
    explicit CCountDownLatch(int iCount);
    void Wait();
    void CountDown();
    int GetCount() const;

private:
    mutable CMutexLock m_clsMutex;
    CCondition m_clsCondition GUARDED_BY(m_clsMutex);
    int m_iCount GUARDED_BY(m_clsMutex);
};


#endif //NECONET_CCOUNTDOWNLATCH_H
