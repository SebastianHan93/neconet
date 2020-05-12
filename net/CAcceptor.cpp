//
// Created by hc on 2020/5/11.
//

#include "CAcceptor.h"
#include "SocketsOps.h"
#include "CEventLoop.h"
#include "CInetAddress.h"

using namespace neco;
using namespace neco::net;
CAcceptor::CAcceptor(CEventLoop * iEventLoop,const CInetAddress& iListenAddr)
    :m_pLoop(iEventLoop),
     m_iAcceptSocket(sockets::CreateNonblockingOrDie()),
     m_iAcceptChannel(iEventLoop,m_iAcceptSocket.GetSocketFd()),
     m_bListenning(false)
{
    m_iAcceptSocket.SetReuseAddr(true);
    m_iAcceptSocket.BindAddredd(iListenAddr);
    m_iAcceptChannel.SetReadCallback(std::bind(&CAcceptor::__HandleRead,this));
}

void CAcceptor::SetNewConnectionCallback(const NEW_CONNECTION_CALLBACK & cb)
{
    m_ifnNewConnectionCallback = cb;
}

bool CAcceptor::IsListenning() const
{
    return m_bListenning;
}

void CAcceptor::StartListen()
{
    m_pLoop->AssertInLoopThread();
    m_bListenning = true;
    m_iAcceptSocket.Listen();
    m_iAcceptChannel.SetEnableReading();
}

void CAcceptor::__HandleRead()
{
    m_pLoop->AssertInLoopThread();
    CInetAddress iPeerAddr(0);
    int nConnFd = m_iAcceptSocket.Accept(&iPeerAddr);
    if(nConnFd >= 0)
    {
        if(m_ifnNewConnectionCallback)
        {
            m_ifnNewConnectionCallback(nConnFd,iPeerAddr);
        }
        else
        {
            sockets::Close(nConnFd);
        }
    }
}