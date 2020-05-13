//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CPOLLER_H
#define NECONET_CPOLLER_H

#include <vector>
#include <map>
#include "../base/noncopyable.h"
#include "../base/CTimestamp.h"

struct pollfd;
namespace neco
{

    namespace net
    {
        class CChannel;
        class CEventLoop;
        class CPoller:noncopyable
        {
        public:
            CPoller(CEventLoop * Eventloop);
            ~CPoller();

        public:
            typedef std::vector<CChannel *>CHANNEL_LIST;
            CTimestamp Poll(int nTimeoutMs,CHANNEL_LIST * ActiveChannels);
            void UpdateChannel(CChannel* iChannel);
            void RemoveChannel(CChannel * iChannel);
            void AssertInLoopThread();

        private:
            void __FillActiveChannels(int nNumEvents,CHANNEL_LIST * ActiveChannels) const;

        private:
            typedef std::vector<struct pollfd> POLL_FD_LIST;
            typedef std::map<int,CChannel*> CHANNEL_MAP;
            CEventLoop * m_pOwnerLoop;
            POLL_FD_LIST m_vPollFds;
            CHANNEL_MAP m_mChannelsMap;

        };
    }
}




#endif //NECONET_CPOLLER_H
