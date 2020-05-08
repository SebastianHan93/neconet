//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CCHANNEL_H
#define NECONET_CCHANNEL_H

#include <functional>
#include "../base/noncopyable.h"
#include "../base/CTimestamp.h"

namespace neco
{
    namespace net
    {
        class CEventloop;

        class CChannel:noncopyable
        {
        public:
            typedef std::function<void()> EVENT_CALLBACK;
            typedef std::function<void(CTimestamp)> READ_EVENT_CALLBACK;

            CChannel(CEventloop* EventLoop,int nFd);
            ~CChannel();

        public:
            void HandleEvent(CTimestamp ReceiveTime);
            void SetReadCallback(READ_EVENT_CALLBACK Callback);
            void SetWriteCallback(EVENT_CALLBACK Callback);
            void SetErrorCallback(EVENT_CALLBACK Callback);

            int GetFd() const;
            int GetEvents() const;
            void SetREvents(int nREvent);
            bool IsNoneEvent() const;
            void EnableReading() const;
            int GetIndex();
            void SetIndex(int nIndex);
            CEventloop * GetOwnerLoop();

        private:
            void __UpdateChannel();

        private:
            static const int sm_nNoneEvent;
            static const int sm_nReadEvent;
            static const int sm_nWriteEvent;

            CEventloop * m_pEventLoop;
            const int m_nFd;
            int m_nEvents;
            int m_nREvents;
            int m_nIndex;
            READ_EVENT_CALLBACK m_ifnReadCallbackFunc;
            EVENT_CALLBACK m_ifnWriteCallbackFunc;
            EVENT_CALLBACK m_ifnErrorCallbackFunc;

        };
    }
}



#endif //NECONET_CCHANNEL_H
