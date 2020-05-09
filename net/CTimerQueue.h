//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CTIMERQUEUE_H
#define NECONET_CTIMERQUEUE_H

#include <set>
#include <vector>
#include "../base/noncopyable.h"
#include "Callbacks.h"
#include "../base/CTimestamp.h"
#include "CChannel.h"
#include "../base/CMutex.h"

namespace neco
{
    namespace net
    {
        class CEventLoop;
        class CTimer;
        class CTimerId;

        class CTimerQueue :noncopyable
        {
        public:
            CTimerQueue(CEventLoop * iEventLoop);
            ~CTimerQueue();

        public:
            CTimerId AddTimer(TIMER_CALL_BACK cb,CTimestamp iWhen, double dfInterval);

        private:
            typedef std::pair<CTimestamp,CTimer*> ENTRY;
            typedef std::set<ENTRY> TIMER_LIST;
            void __HandleRead();
            std::vector<ENTRY> __GetExpired(CTimestamp iNow);
            void __ResetTimers(const std::vector<ENTRY> & vExpired,CTimestamp iNow);
            bool __InsertTimer(CTimer * iTimer);
        private:
            CEventLoop* m_pEventLoop;
            const int m_nTimerfd;
            CChannel m_iTimerFdChannel;
            TIMER_LIST m_ssTimers;
        };
    }
}



#endif //NECONET_CTIMERQUEUE_H
