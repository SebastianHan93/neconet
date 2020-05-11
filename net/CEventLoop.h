//
// Created by hc on 2020/5/7.
//

#ifndef NECONET_CEVENTLOOP_H
#define NECONET_CEVENTLOOP_H


#include <vector>
#include <bits/unique_ptr.h>
#include "../base/noncopyable.h"
#include "../base/Types.h"
#include "../base/CTimestamp.h"
#include "Callbacks.h"
#include "CTimerId.h"
#include "../base/CMutex.h"

namespace neco
{
    namespace net
    {
        class CChannel;
        class CPoller;
        class CTimerQueue;
        class CEventLoop : noncopyable
        {
        public:
            CEventLoop();
            ~CEventLoop();

        public:
            typedef std::function<void()> FUNCTOR;
            void StartLoop();
            void QuitLoop();
            void AssertInLoopThread();
            bool IsInLoopThread() const ;
            static CEventLoop* GetEventLoopOfCurrentThread();
            void UpdateChannel(CChannel * iChannel);
            CTimestamp PollReturnTime() const;
            CTimerId RunAt(CTimestamp iTime,TIMER_CALL_BACK cb);
            CTimerId RunAfter(double dfDelay,TIMER_CALL_BACK cb);
            CTimerId RunEvery(double dfInterval,TIMER_CALL_BACK cb);
            void RunInLoop(FUNCTOR cb);
            void QueueInLoop(FUNCTOR cb);
            void Wakeup();


        private:
            void __AbortNotInLoopThread();
            void __HandleRead();
            void __DoPendingFunctors();

        private:
            typedef std::vector<CChannel*> CHANNEL_LIST;
            bool m_bInLooping;
            bool m_bQuit;
            bool m_bCallingPendingFunctors;
            const pid_t m_pidThreadId;
            CTimestamp m_iPollReturnTime;
            std::unique_ptr<CPoller> m_unpPoller;
            std::unique_ptr<CTimerQueue> m_unpTimerQueue;
            CHANNEL_LIST m_vActiveChannels;
            int m_nWakeupFd;
            std::unique_ptr<CChannel> m_unpWakeupChannel;
            CMutexLock m_iMutex;
            std::vector<FUNCTOR> m_vPendingFunctors GUARDED_BY(m_iMutex);


        };
    }
}



#endif //NECONET_CEVENTLOOP_H
