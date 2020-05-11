//
// Created by hc on 2020/5/11.
//

#include "CInetAddress.h"
#include "SocketsOps.h"
#include <strings.h>

using namespace neco;
using namespace neco::net;

static const in_addr_t iInAddrAny = INADDR_ANY;


CInetAddress::CInetAddress(uint64_t nPort)
{
    bzero(&m_iAddr, sizeof(m_iAddr));
    m_iAddr.sin_family = AF_INET;
    m_iAddr.sin_addr.s_addr = sockets::HostToNetwork32(iInAddrAny);
    m_iAddr.sin_port = sockets::HostToNetwork16(nPort);
}

CInetAddress::CInetAddress(const std::string & rPp,uint64_t nPort)
{
    bzero(&m_iAddr, sizeof(m_iAddr));
    sockets::FromHostPort(rPp.c_str(),nPort,&m_iAddr);
}

CInetAddress::CInetAddress(const struct sockaddr_in& iAddr)
        :m_iAddr(iAddr)
{

}

std::string CInetAddress::ToHostPort() const
{
    char cBuf[32];
    sockets::ToHostPort(cBuf, sizeof cBuf, m_iAddr);
    return cBuf;
}

const struct sockaddr_in & CInetAddress::GetSockAddrInet() const
{
    return m_iAddr;
}

void CInetAddress::SetSockAddrInet(const struct sockaddr_in& iAddr)
{
    m_iAddr = iAddr;
}