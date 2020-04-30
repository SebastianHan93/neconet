//
// Created by hc on 2020/4/29.
//

#ifndef NECONET_CCONDITION_H
#define NECONET_CCONDITION_H


#include "noncopyable.h"
#include "CMutex.h"

namespace neco
{
    class CCondition : noncopyable{
    public:
        explicit CCondition(CMutexLock & rMutexLock);
        ~CCondition();
        void Wait();
        bool WaitForSeconds(double dSeconds);
        void Notify();
        void NotifyAll();
    private:
        CMutexLock & m_rMutex;
        pthread_cond_t m_pctCond;
    };
}

#endif //NECONET_CCONDITION_H
