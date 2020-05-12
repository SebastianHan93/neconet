//
// Created by hc on 2020/5/11.
//

#ifndef NECONET_CTCPSERVER_H
#define NECONET_CTCPSERVER_H

#include <map>
#include "../base/noncopyable.h"
#include "CTcpConnection.h"
#include "Callbacks.h"

namespace neco
{
    namespace net
    {
        class CAcceptor;
        class CEventLoop;
//        class EventLoopThreadPool;
        class CTcpServer :noncopyable
        {
        public:
            CTcpServer(CEventLoop * pLoop,const CInetAddress & iListenAddr);
            ~CTcpServer();

        public:
            void Start();
            void SetConnectionCallback(const CONNECTION_CALLBACK & cb);
            void SetMessageCallback(const MESSAGE_CALLBACK & cb);

        private:
            void __NewConnection(int nSockFd,const CInetAddress &iPeerAddr);

        private:
            typedef std::map<std::string,C_TCP_CONNECTION_PTR> CONNECTION_MAP;
            CEventLoop * m_pEventLoop;
            const std::string m_sName;
            std::unique_ptr<CAcceptor> m_unpAcceptor;
            CONNECTION_CALLBACK m_ifnConnectionCallback;
            MESSAGE_CALLBACK m_ifnMessageCallback;
            bool m_bStarted;
            int m_nNextConnId;
            CONNECTION_MAP m_mConnectionMap;
        };
    }
}



#endif //NECONET_CTCPSERVER_H
