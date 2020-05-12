//
// Created by hc on 2020/5/11.
//

#ifndef NECONET_CACCEPTOR_H
#define NECONET_CACCEPTOR_H

#include <functional>
#include "../base/noncopyable.h"
#include "CChannel.h"
#include "CSocket.h"


namespace neco
{
    namespace net
    {
        class CEventLoop;
        class CInetAddress;
        class CAcceptor:noncopyable
        {
        public:
            typedef std::function<void (int nSockFd, const CInetAddress&)> NEW_CONNECTION_CALLBACK;
            CAcceptor(CEventLoop * iEventLoop,const CInetAddress& iListenAddr);

        public:
            void SetNewConnectionCallback(const NEW_CONNECTION_CALLBACK &cb);
            bool IsListenning() const;
            void StartListen();

        private:
            void __HandleRead();

        private:
            CEventLoop * m_pLoop;
            CSocket m_iAcceptSocket;
            CChannel m_iAcceptChannel;
            NEW_CONNECTION_CALLBACK m_ifnNewConnectionCallback;
            bool m_bListenning;
        };
    }
}



#endif //NECONET_CACCEPTOR_H
