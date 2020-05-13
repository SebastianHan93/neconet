//
// Created by hc on 2020/5/11.
//
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>
#include "CSocket.h"
#include "SocketsOps.h"
#include "CInetAddress.h"
#include <iostream>

using namespace neco;
using namespace neco::net;

CSocket::CSocket(int nSockFd)
    :m_nSockFd(nSockFd)
{

}

CSocket::~CSocket()
{
    sockets::Close(m_nSockFd);
}

int CSocket::GetSocketFd() const
{
    return m_nSockFd;
}

void CSocket::BindAddredd(const CInetAddress& iLocalAddr)
{
    sockets::BindOrDie(m_nSockFd,iLocalAddr.GetSockAddrInet());
}

void CSocket::Listen()
{
    sockets::ListenOrDie(m_nSockFd);
}

int CSocket::Accept(CInetAddress * iPeerAddr)
{
    struct sockaddr_in iAddr;
    bzero(&iAddr, sizeof(iAddr));
    int nConnFd = sockets::Accept(m_nSockFd,&iAddr);
    if(nConnFd>=0)
    {
        iPeerAddr->SetSockAddrInet(iAddr);
    }
    return nConnFd;
}

void CSocket::SetReuseAddr(bool bOn)
{
    int nOptVal = bOn ? 1:0;
    ::setsockopt(m_nSockFd,SOL_SOCKET,SO_REUSEADDR,&nOptVal, sizeof(nOptVal));
}

void CSocket::ShutdownWrite()
{
    sockets::ShutdownWrite(m_nSockFd);
}