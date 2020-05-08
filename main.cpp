#include <iostream>
#include <map>
#include <stdio.h>
#include "net/CEventLoop.h"
#include "base/CThread.h"
#include "base/CurrentThread.h"
#include <sys/types.h>
#include <unistd.h>


using namespace neco;
using namespace neco::net;

CEventLoop * g_iEventloop;
void threadFunc()
{
    g_iEventloop->StartLoop();
}

int main ()
{
    CEventLoop loop;
    g_iEventloop = &loop;
    CThread t(threadFunc);
    t.Start();
    t.ThreadJoin();
}
