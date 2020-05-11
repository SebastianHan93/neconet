//
// Created by hc on 2020/5/11.
//

#include "CSocket.h"

using namespace neco;
using namespace neco::net;

CSocket::CSocket(int nSockFd)
    :m_nSockFd(m_nSockFd)
{

}

CSocket::~CSocket()
{

}

int CSocket::GetSocketFd() const
{

}

void CSocket::BindAddredd(const CInetAddress& iLocalAddr)
{

}

void CSocket::Listen()
{

}

int CSocket::Accept(CInetAddress * iPeerAddr)
{

}

void CSocket::SetReuseAddr(bool bOn)
{

}