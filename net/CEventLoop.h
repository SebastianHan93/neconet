//
// Created by hc on 2020/5/7.
//

#ifndef NECONET_CEVENTLOOP_H
#define NECONET_CEVENTLOOP_H


#include <vector>
#include <bits/unique_ptr.h>
#include "../base/noncopyable.h"
#include "../base/Types.h"
namespace neco
{
    namespace net
    {
        class CChannel;
        class CPoller;

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

        private:
            void __AbortNotInLoopThread();

        private:
            typedef std::vector<CChannel*> CHANNEL_LIST;
            bool m_bInLooping;
            bool m_bQuit;
            const pid_t m_pidThreadId;
            std::unique_ptr<CPoller> m_unpPoller;
            CHANNEL_LIST m_vActiveChannels;
        };
    }
}



#endif //NECONET_CEVENTLOOP_H
