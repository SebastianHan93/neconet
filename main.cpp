#include <iostream>
#include <map>
#include <stdio.h>
#include "net/CEventLoop.h"
#include "base/CThread.h"
#include "base/CurrentThread.h"
#include "net/CChannel.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/timerfd.h>


using namespace neco;
using namespace neco::net;

CEventLoop * g_iEventloop;
void timeout(CTimestamp timestamp)
{
    printf("Timeout!\n");
    g_iEventloop->QuitLoop();
}

int main ()
{
    CEventLoop loop;
    g_iEventloop = &loop;
    int timerFd = ::timerfd_create(CLOCK_REALTIME,TFD_NONBLOCK|TFD_CLOEXEC);
    CChannel  channel(&loop,timerFd);
    channel.SetReadCallback(timeout);
    channel.SetEnableReading();
    struct itimerspec howlang;
    bzero(&howlang,sizeof(howlang));
    howlang.it_value.tv_sec = 1;
    ::timerfd_settime(timerFd,TFD_TIMER_ABSTIME,&howlang, NULL);
    loop.UpdateChannel(&channel);
    loop.StartLoop();
    ::close(timerFd);



}
