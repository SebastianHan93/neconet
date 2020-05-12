//
// Created by hc on 2020/5/11.
//

#ifndef NECONET_CTCPCONNECTION_H
#define NECONET_CTCPCONNECTION_H

#include <memory>
#include "../base/noncopyable.h"
#include "Callbacks.h"
#include "CInetAddress.h"

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;
namespace neco
{
    namespace net
    {
        class CChannel;
        class CEventLoop;
        class CSocket;
        class CTcpConnection:noncopyable,public std::enable_shared_from_this<CTcpConnection>
        {
        public:
            CTcpConnection(CEventLoop * pLoop,const std::string& csName,int nSockFd,const CInetAddress & iLocalAddr,const CInetAddress & iPeerAddr);
            ~CTcpConnection();

        public:
            CEventLoop * GetLoop() const;
            const std::string & GetName();
            const CInetAddress & GetLocalAddress();
            const CInetAddress & GetPeerAddress();
            bool IsConnected() const;
            void SetConnectionCallback(const CONNECTION_CALLBACK cb);
            void SetMessageCallback(const MESSAGE_CALLBACK cb);
            void ConnectEstablished();

        private:
            enum ESTATE{eConnecting,eConnected};
            void __HandleRead();
            void __SetState(ESTATE eState);
        private:
            CEventLoop * m_pLoop;
            std::string m_sName;
            ESTATE m_eState;
            std::unique_ptr<CSocket> m_unpSocket;
            std::unique_ptr<CChannel> m_unpChannel;
            CInetAddress m_iLocalAddr;
            CInetAddress m_iPeerAddr;
            CONNECTION_CALLBACK m_ifnConnectionCallback;
            MESSAGE_CALLBACK m_ifnMessageCallback;
        };
        typedef std::shared_ptr<CTcpConnection> C_TCP_CONNECTION_PTR;
    }
}



#endif //NECONET_CTCPCONNECTION_H
