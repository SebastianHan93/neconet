//
// Created by hc on 2020/5/8.
//

#include "CPoller.h"
#include "CChannel.h"
#include "CEventLoop.h"

using namespace neco;
using namespace neco::net;

CPoller::CPoller(CEventLoop * Eventloop):m_pOwnerLoop(Eventloop)
{

}