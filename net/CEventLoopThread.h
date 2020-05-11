//
// Created by hc on 2020/5/11.
//

#ifndef NECONET_CEVENTLOOPTHREAD_H
#define NECONET_CEVENTLOOPTHREAD_H

#include "../base/noncopyable.h"
#include "../base/CThread.h"

namespace neco
{
    namespace net
    {
        class CEventLoop;
        class CEventLoopThread : noncopyable
        {
        public:
             CEventLoopThread();
             ~CEventLoopThread();

        public:
             CEventLoop * StartLoop();

        private:
            void __ThreadFunc();

        private:
            CEventLoop * m_iEventLoop;
            bool m_bExiting;
            CThread m_iThread;
            CMutexLock m_iMutex;
            CCondition m_iCond;
        };
    }
}



#endif //NECONET_CEVENTLOOPTHREAD_H
