//
// Created by hc on 2020/5/11.
//
#include "SocketsOps.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  // snprintf
#include <strings.h>  // bzero
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "../base/Types.h"

using namespace neco;
using namespace neco::net;

namespace
{
    typedef struct sockaddr SA;
    const SA* sockaddr_cast(const struct sockaddr_in * iAddr)
    {
        return static_cast<const SA*>(implicit_cast<const void *>(iAddr));
    }

    SA* sockaddr_cast(struct sockaddr_in* iAddr)
    {
        return static_cast<SA*>(implicit_cast<void *>(iAddr));
    }

    void SetNonBlockAndCloseOnExec(int nSockfd)
    {
        // non-block
        int flags = ::fcntl(nSockfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        int ret = ::fcntl(nSockfd, F_SETFL, flags);
        // FIXME check

        // close-on-exec
        flags = ::fcntl(nSockfd, F_GETFD, 0);
        flags |= FD_CLOEXEC;
        ret = ::fcntl(nSockfd, F_SETFD, flags);
        // FIXME check
    }
}

int sockets::CreateNonblockingOrDie()
{

#if VALGRIND
    int nSockFd = ::socket(family,SOCK_STREAM,IPPROTO_TCP);
    if(nSockFd < 0)
    {
       std::cout <<  "sockets::createNonblockingOrDie" << std::endl;
    }
    SetNonBlockAndCloseOnExec(nSockFd);
#else
    int nSockFd = ::socket(AF_INET,
                          SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                          IPPROTO_TCP);
    if (nSockFd < 0)
    {
        std::cout << "sockets::createNonblockingOrDie" << std::endl;
    }
#endif
}

void sockets::BindOrDie(int nSockFd,const struct sockaddr_in & iAddr)
{
    int nRet = ::bind(nSockFd,sockaddr_cast(&iAddr), sizeof(iAddr));
    if(nRet < 0)
    {
        std::cout << "sockets::bindOrDie" << std::endl;
    }
}

void sockets::ListenOrDie(int nSockFd)
{
    int nRet = ::listen(nSockFd, SOMAXCONN);
    if (nRet < 0)
    {
        std::cout << "sockets::listenOrDie" << std::endl;
    }
}

int sockets::Accept(int nSockFd,struct sockaddr_in* iAddr)
{
    socklen_t iAddrLen = sizeof(iAddr);
#if VALGRIND
    int nConnFd = ::accept(nSockFd,sockaddr_cast(iAddr),&iAddrLen);
    SetNonBlockAndCloseOnExec(nConnFd);
#else
    int nConnFd = ::accept4(nSockFd,sockaddr_cast(iAddr),&iAddrLen,SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if(nConnFd<0)
    {
        int nSavedErrno = errno;
        std::cout << "Socket::accept" << std::endl;
        switch (nSavedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO: // ???
            case EPERM:
            case EMFILE: // per-process lmit of open file desctiptor ???
                // expected errors
                errno = nSavedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                std::cout << "unexpected error of ::accept " << nSavedErrno << std::endl;
                break;
            default:
                std::cout << "unknown error of ::accept " << nSavedErrno << std::endl;
                break;
        }
        return nConnFd;
    }
}

void sockets::Close(int nSockFd)
{
    if (::close(nSockFd) < 0)
    {
        std::cout << "sockets::close" <<std::endl;
    }
}

void sockets::ToHostPort(char * cBuf,size_t nSize,const struct sockaddr_in& iAddr)
{
    char cHost[INET_ADDRSTRLEN] = "INVALID";
    ::inet_ntop(AF_INET, &iAddr.sin_addr, cHost, sizeof cHost);
    uint16_t port = sockets::NetworkToHost16(iAddr.sin_port);
    snprintf(cBuf, nSize, "%s:%u", cHost, port);
}

void sockets::FromHostPort(const char * pIP,uint16_t nPort, struct sockaddr_in * iAddr)
{
    iAddr->sin_family = AF_INET;
    iAddr->sin_port = HostToNetwork16(nPort);
    if (::inet_pton(AF_INET, pIP, &iAddr->sin_addr) <= 0)
    {
        std::cout << "sockets::fromHostPort" <<std::endl;
    }
}

struct sockaddr_in sockets::GetLocalAddr(int nSockfd)
{
    struct sockaddr_in iLocalAddr;
    bzero(&iLocalAddr, sizeof(iLocalAddr));
    socklen_t nAddrLen = sizeof(iLocalAddr);
    if(::getsockname(nSockfd,sockaddr_cast(&iLocalAddr),&nAddrLen)<0)
    {
        std::cout << "sockets::GetLocalAddr";
    }

    return iLocalAddr;
}