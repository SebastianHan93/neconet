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
        class CEventLoop;

        class CChannel:noncopyable
        {
        public:
            typedef std::function<void()> EVENT_CALLBACK;
            typedef std::function<void(CTimestamp)> READ_EVENT_CALLBACK;

            CChannel(CEventLoop* EventLoop,int nFd);
            ~CChannel();

        public:
            void HandleEvent(CTimestamp ReceiveTime);
            void SetReadCallback(READ_EVENT_CALLBACK Callback);
            void SetWriteCallback(EVENT_CALLBACK Callback);
            void SetErrorCallback(EVENT_CALLBACK Callback);
            void SetCloseCallback(EVENT_CALLBACK Callback);

            int GetFd() const;
            int GetEvents() const;
            void SetREvents(int nREvent);
            bool IsNoneEvent() const;

            void SetEnableReading();
            void SetEnableWriting();
            void DisableWriting();
            void DisableAll();
            bool IsWriting();

            int GetIndex();
            void SetIndex(int nIndex);
            CEventLoop * GetOwnerLoop();


        private:
            void __UpdateChannel();

        private:
            static const int sm_nNoneEvent;
            static const int sm_nReadEvent;
            static const int sm_nWriteEvent;

            CEventLoop * m_pEventLoop;
            const int m_nFd;
            int m_nEvents;
            int m_nREvents;
            int m_nIndex;

            bool m_bEventHandling;

            READ_EVENT_CALLBACK m_ifnReadCallbackFunc;
            EVENT_CALLBACK m_ifnWriteCallbackFunc;
            EVENT_CALLBACK m_ifnErrorCallbackFunc;
            EVENT_CALLBACK m_ifnCloseCallbackFunc;
        };
    }
}



#endif //NECONET_CCHANNEL_H
