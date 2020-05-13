//
// Created by hc on 2020/5/8.
//

#include "CChannel.h"
#include "CEventLoop.h"
#include <poll.h>

using namespace neco;
using namespace neco::net;

const int CChannel::sm_nNoneEvent = 0;
const int CChannel::sm_nReadEvent = POLLIN | POLLPRI;
const int CChannel::sm_nWriteEvent = POLLOUT;

CChannel::CChannel(CEventLoop* EventLoop,int nFd)
    :m_pEventLoop(EventLoop),
     m_nFd(nFd),
     m_nEvents(0),
     m_nREvents(0),
     m_nIndex(-1),
     m_bEventHandling(false)
{

}

CChannel::~CChannel()
{
    assert(!m_bEventHandling);
}

void CChannel::HandleEvent(CTimestamp ReceiveTime)
{
    m_bEventHandling = true;
    if(m_nREvents & POLLNVAL)
    {
        printf("Channel::handle_event() POLLNVAL\n");
    }
    if((m_nREvents & POLLHUP) && !(m_nREvents & POLLIN))
    {
        printf("Channel::handle_event() POLLHUP\n");
        if(m_ifnCloseCallbackFunc)
            m_ifnCloseCallbackFunc();
    }
    if(m_nREvents & (POLLERR|POLLNVAL))
    {
        if(m_ifnErrorCallbackFunc)
            m_ifnErrorCallbackFunc();
    }
    if(m_nREvents&(POLLIN | POLLPRI | POLLRDHUP))
    {
        if(m_ifnReadCallbackFunc)
            m_ifnReadCallbackFunc(ReceiveTime);
    }
    if(m_nREvents&POLLOUT)
    {
        if(m_ifnWriteCallbackFunc)
            m_ifnWriteCallbackFunc();
    }
    m_bEventHandling = false;
}

void CChannel::SetReadCallback(READ_EVENT_CALLBACK Callback)
{
    m_ifnReadCallbackFunc = std::move(Callback);
}

void CChannel::SetWriteCallback(EVENT_CALLBACK Callback)
{
    m_ifnWriteCallbackFunc = std::move(Callback);
}

void CChannel::SetErrorCallback(EVENT_CALLBACK Callback)
{
    m_ifnErrorCallbackFunc = std::move(Callback);
}

void CChannel::SetCloseCallback(EVENT_CALLBACK Callback)
{
    m_ifnCloseCallbackFunc = std::move(Callback);
}

void CChannel::DisableAll()
{
    m_nEvents = sm_nNoneEvent;
    __UpdateChannel();
}

int CChannel::GetFd() const
{
    return m_nFd;
}

int CChannel::GetEvents() const
{
    return m_nEvents;
}

void CChannel::SetREvents(int nREvent)
{
    m_nREvents = nREvent;
}

bool CChannel::IsNoneEvent() const
{
    return static_cast<bool>(m_nEvents == sm_nNoneEvent);
}

void CChannel::SetEnableReading()
{
    m_nEvents |= sm_nReadEvent;
    __UpdateChannel();
}

void CChannel::SetEnableWriting()
{
    m_nEvents |=sm_nWriteEvent;
    __UpdateChannel();
}

void CChannel::DisableWriting()
{
    m_nEvents &= ~sm_nWriteEvent;
    __UpdateChannel();
}

bool CChannel::IsWriting()
{
    return m_nEvents & sm_nWriteEvent;
}

int CChannel::GetIndex()
{
    return m_nIndex;
}

void CChannel::SetIndex(int nIndex)
{
    m_nIndex = nIndex;
}

CEventLoop * CChannel::GetOwnerLoop()
{
    return m_pEventLoop;
}

void CChannel::__UpdateChannel()
{
    m_pEventLoop->UpdateChannel(this);
}