//
// Created by hc on 2020/5/11.
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "CTcpConnection.h"
#include "CSocket.h"
#include "CChannel.h"
#include "CEventLoop.h"

using namespace neco;
using namespace neco::net;
CTcpConnection::CTcpConnection(CEventLoop * pLoop,const std::string& csName,int nSockFd,const CInetAddress & iLocalAddr,const CInetAddress & iPeerAddr)
    :m_pLoop(pLoop),
     m_sName(csName),
     m_eState(eConnecting),
     m_unpSocket(new CSocket(nSockFd)),
     m_unpChannel(new CChannel(pLoop,nSockFd)),
     m_iLocalAddr(iLocalAddr),
     m_iPeerAddr(iPeerAddr)
{
    std::cout << "TcpConnection::ctor[" <<  m_sName << "] at " << this
              << " fd=" << nSockFd << std::endl;
    m_unpChannel->SetReadCallback(std::bind(&CTcpConnection::__HandleRead,this));
}

CTcpConnection::~CTcpConnection()
{
    std::cout << "TcpConnection::dtor[" <<  m_sName << "] at " << this
              << " fd=" << m_unpChannel->GetFd() << std::endl;
}

CEventLoop * CTcpConnection::GetLoop() const
{
    return m_pLoop;
}

const std::string & CTcpConnection::GetName()
{
    return m_sName;
}

const CInetAddress & CTcpConnection::GetLocalAddress()
{
    return m_iLocalAddr;
}

const CInetAddress & CTcpConnection::GetPeerAddress()
{
    return m_iPeerAddr;
}

bool CTcpConnection::IsConnected() const
{
    return m_eState == eConnected;
}

void CTcpConnection::SetConnectionCallback(const CONNECTION_CALLBACK cb)
{
    m_ifnConnectionCallback = cb;
}

void CTcpConnection::SetMessageCallback(const MESSAGE_CALLBACK cb)
{
    m_ifnMessageCallback = cb;
}

void CTcpConnection::ConnectEstablished()
{
    m_pLoop->AssertInLoopThread();
    assert(m_eState == eConnecting);
    __SetState(eConnected);
    m_unpChannel->SetEnableReading();
    m_ifnConnectionCallback(shared_from_this());
}

void CTcpConnection::__HandleRead()
{
    char cBuf[65535];
    ssize_t n = ::read(m_unpChannel->GetFd(),cBuf, sizeof(cBuf));
    m_ifnMessageCallback(shared_from_this(),cBuf,n);
}

void CTcpConnection::__SetState(ESTATE eState)
{
    m_eState = eState;
}