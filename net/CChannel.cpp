//
// Created by hc on 2020/5/8.
//

#include "CChannel.h"
#include <poll.h>

using namespace neco;
using namespace neco::net;

const int CChannel::sm_nNoneEvent = 0;
const int CChannel::sm_nReadEvent = POLLIN | POLLPRI;
const int CChannel::sm_nWriteEvent = POLLOUT;

CChannel::CChannel(CEventloop* EventLoop,int nFd)
:m_pEventLoop(EventLoop),m_nFd(nFd),m_nEvents(0),m_nREvents(0),m_nIndex(-1)
{

}

CChannel::~CChannel()
{

}

void CChannel::HandleEvent(CTimestamp ReceiveTime)
{

}

void CChannel::SetReadCallback(READ_EVENT_CALLBACK Callback)
{

}

void CChannel::SetWriteCallback(EVENT_CALLBACK Callback)
{

}

void CChannel::SetErrorCallback(EVENT_CALLBACK Callback)
{

}

int CChannel::GetFd() const
{

}

int CChannel::GetEvents() const
{

}

void CChannel::SetREvents(int nREvent)
{

}

bool CChannel::IsNoneEvent() const
{

}

void CChannel::EnableReading() const
{

}

int CChannel::GetIndex()
{

}

void CChannel::SetIndex(int nIndex)
{

}

CEventloop * CChannel::GetOwnerLoop()
{

}

void CChannel::__UpdateChannel()
{

}