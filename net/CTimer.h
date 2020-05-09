//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CTIMER_H
#define NECONET_CTIMER_H

#include "../base/noncopyable.h"
#include "Callbacks.h"
#include "../base/CTimestamp.h"

namespace neco
{
    namespace net
    {
        class CTimer:noncopyable
        {
        public:
            CTimer(const TIMER_CALL_BACK &cb,CTimestamp iWhen, double dfInterval);

        public:
            void Run() const ;
            CTimestamp GetExpiration() const;
            bool GetRepeat();
            void Restart(CTimestamp iNowTime);

        private:
            const TIMER_CALL_BACK m_ifnCallback;
            CTimestamp m_iExpiration;
            const double m_dfInterval;
            const bool m_bRepeat;
        };
    }
}



#endif //NECONET_CTIMER_H
