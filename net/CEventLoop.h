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

        private:
            void __AbortNotInLoopThread();

        private:
            typedef std::vector<CChannel*> CHANNEL_LIST;
            bool m_bInLooping;
            bool m_bQuit;
            const pid_t m_pidThreadId;
            CTimestamp m_iPollReturnTime;
            std::unique_ptr<CPoller> m_unpPoller;
            std::unique_ptr<CTimerQueue> m_unpTimerQueue;
            CHANNEL_LIST m_vActiveChannels;

        };
    }
}



#endif //NECONET_CEVENTLOOP_H
