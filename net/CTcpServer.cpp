//
// Created by hc on 2020/5/11.
//

#include <functional>
#include <stdio.h>

#include "CTcpServer.h"
#include "CAcceptor.h"
#include "CEventLoop.h"
#include "SocketsOps.h"

using namespace neco;
using namespace neco::net;
using std::placeholders::_1;
using std::placeholders::_2;

CTcpServer::CTcpServer(CEventLoop * pLoop,const CInetAddress & iListenAddr)
    :m_pEventLoop(pLoop),
     m_sName(iListenAddr.ToHostPort()),
     m_unpAcceptor(new CAcceptor(pLoop,iListenAddr)),
     m_bStarted(false),
     m_nNextConnId(1)
{
    m_unpAcceptor->SetNewConnectionCallback(std::bind(&CTcpServer::__NewConnection,this,_1,_2));
}

CTcpServer::~CTcpServer()
{
    m_pEventLoop->AssertInLoopThread();
}

void CTcpServer::Start()
{
    if(!m_bStarted)
    {
        m_bStarted = true;
    }
    if(!m_unpAcceptor->IsListenning())
    {
        m_pEventLoop->RunInLoop(std::bind(&CAcceptor::StartListen,get_pointer(m_unpAcceptor)));
    }
}

void CTcpServer::SetConnectionCallback(const CONNECTION_CALLBACK & cb)
{
    m_ifnConnectionCallback = cb;
}

void CTcpServer::SetMessageCallback(const MESSAGE_CALLBACK & cb)
{
    m_ifnMessageCallback = cb;
}

void CTcpServer::__NewConnection(int nSockFd,const CInetAddress &iPeerAddr)
{
    m_pEventLoop->AssertInLoopThread();
    char cBuf[32];
    snprintf(cBuf,sizeof(cBuf),"#%d",m_nNextConnId);
    ++m_nNextConnId;
    std::string sConnName = m_sName + cBuf;
    printf("TcpServer::newConnection [%s] - new connection [%s] from  %s\n",m_sName.c_str(),sConnName.c_str(),iPeerAddr.ToHostPort().c_str());
    CInetAddress iLocalAddr(sockets::GetLocalAddr(nSockFd));
    C_TCP_CONNECTION_PTR sspConn(new CTcpConnection(m_pEventLoop,sConnName,nSockFd,iLocalAddr,iPeerAddr));
    m_mConnectionMap[sConnName] = sspConn;
    sspConn->SetConnectionCallback(m_ifnConnectionCallback);
    sspConn->SetMessageCallback(m_ifnMessageCallback);
    sspConn->SetCloseCallback(std::bind(&CTcpServer::__RemoveConnection,this,_1));
    sspConn->ConnectEstablished();
}

void CTcpServer::__RemoveConnection(const C_TCP_CONNECTION_PTR & iConn)
{
    m_pEventLoop->AssertInLoopThread();
    printf("TcpServer::removeConnection [%s] - connection [%s]",m_sName.c_str(),iConn->GetName().c_str());
    size_t n = m_mConnectionMap.erase(iConn->GetName());
    assert(n==1);(void)n;
    m_pEventLoop->QueueInLoop(std::bind(&CTcpConnection::ConnectDestroyed,iConn));
}