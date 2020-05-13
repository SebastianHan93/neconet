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
#include "SocketsOps.h"

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
    m_unpChannel->SetReadCallback(std::bind(&CTcpConnection::__HandleRead,this,std::placeholders::_1));
    m_unpChannel->SetWriteCallback(std::bind(&CTcpConnection::__HandleWrite,this));
    m_unpChannel->SetCloseCallback(std::bind(&CTcpConnection::__HandleClose,this));
    m_unpChannel->SetErrorCallback(std::bind(&CTcpConnection::__HandleError,this));
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

void CTcpConnection::SetConnectionCallback(const CONNECTION_CALLBACK &cb)
{
    m_ifnConnectionCallback = cb;
}

void CTcpConnection::SetMessageCallback(const MESSAGE_CALLBACK &cb)
{
    m_ifnMessageCallback = cb;
}

void CTcpConnection::SetCloseCallback(const CLOSE_CALLBACK &cb)
{
    m_ifnCloseCallback = cb;
}


void CTcpConnection::ConnectEstablished()
{
    m_pLoop->AssertInLoopThread();
    assert(m_eState == eConnecting);
    __SetState(eConnected);
    m_unpChannel->SetEnableReading();
    m_ifnConnectionCallback(shared_from_this());
}

void CTcpConnection::ConnectDestroyed()
{
    m_pLoop->AssertInLoopThread();
    assert(m_eState == eConnected || m_eState == eDisconnecting);
    __SetState(eDisconnected);
    m_unpChannel->DisableAll();
    m_ifnConnectionCallback(shared_from_this());
    m_pLoop->RemoveChannel(get_pointer(m_unpChannel));
}

void CTcpConnection::__HandleWrite()
{
    m_pLoop->AssertInLoopThread();
    if(m_unpChannel->IsWriting())
    {
        ssize_t n = ::write(m_unpChannel->GetFd(),m_iOutputBuffer.Peek(),m_iOutputBuffer.GetReadableBytes());
        if(n>0)
        {
            m_iOutputBuffer.Retrieve(n);
            if(m_iOutputBuffer.GetReadableBytes() == 0)
            {
                m_unpChannel->DisableWriting();
                if(m_eState == eDisconnecting)
                {
                    __ShutdownInLoop();
                }
            }
            else
            {
                printf("I am going to write more data\n");
            }
        }
        else
        {
            printf("TcpConnection::handleWrite\n");
        }
    }
    else
    {
        printf("Connection is down, no more writing\n");
    }
}

void CTcpConnection::__HandleClose()
{
    m_pLoop->AssertInLoopThread();
    printf("TcpConnection::handleClose state = %d",m_eState);
    assert(m_eState == eConnected || m_eState == eDisconnecting);
    // we don't close fd, leave it to dtor, so we can find leaks easily.
    m_unpChannel->DisableAll();
    // must be the last line
    m_ifnCloseCallback(shared_from_this());
}

void CTcpConnection::__HandleError()
{
    int nErr = sockets::GetSocketError(m_unpChannel->GetFd());
    printf("CTcpConnection::__HandleError [%s] - SO_ERROR = %d ",m_sName.c_str(),nErr);
}

void CTcpConnection::__HandleRead(CTimestamp iTimestamp)
{
    int nSavedErrno = 0;
    ssize_t n = m_iInputBuffer.ReadFd(m_unpChannel->GetFd(),&nSavedErrno);
    if(n>0)
    {
        m_ifnMessageCallback(shared_from_this(),&m_iInputBuffer,iTimestamp);
    }
    else if(n == 0)
    {
        __HandleClose();
    }
    else
    {
        errno = nSavedErrno;
        printf("CTcpConnection::__HandleRead");
        __HandleError();
    }
}

void CTcpConnection::__SetState(ESTATE eState)
{
    m_eState = eState;
}

void CTcpConnection::Send(const std::string& rMessage)
{
    if(m_eState == eConnected)
    {
        if(m_pLoop->IsInLoopThread())
        {
            __SendInLoop(rMessage);
        }
        else
        {
            m_pLoop->RunInLoop(std::bind(&CTcpConnection::__SendInLoop,this,rMessage));
        }
    }
}

void CTcpConnection::ShutDown()
{
    if(m_eState == eConnected)
    {
        __SetState(eDisconnecting);
        m_pLoop->RunInLoop(std::bind(&CTcpConnection::__ShutdownInLoop,this));
    }
}

void CTcpConnection::__SendInLoop(const std::string & rMessage)
{
    m_pLoop->AssertInLoopThread();
    ssize_t nWrote = 0;
    if(!m_unpChannel->IsWriting() && m_iOutputBuffer.GetReadableBytes()==0)
    {
        nWrote = ::write(m_unpChannel->GetFd(),rMessage.data(),rMessage.size());
        if(nWrote >=0)
        {
            if(implicit_cast<size_t >(nWrote) < rMessage.size())
            {
                printf("I am going to write more data\n");
            }
        }
        else
        {
            nWrote = 0;
            if(errno != EWOULDBLOCK)
            {
                printf("CTcpConnection::__SendInLoop");
            }
        }
    }
    assert(nWrote>=0);
    if(implicit_cast<size_t>(nWrote) < rMessage.size())
    {
        m_iOutputBuffer.Append(rMessage.data()+nWrote,rMessage.size()-nWrote);
        if(!m_unpChannel->IsWriting())
        {
            m_unpChannel->SetEnableReading();
        }
    }
}

void CTcpConnection::__ShutdownInLoop()
{
    m_pLoop->AssertInLoopThread();
    if(!m_unpChannel->IsWriting())
    {
        m_unpSocket->ShutdownWrite();
    }
}