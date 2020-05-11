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
using namespace neco::net;
using namespace neco;
CEventLoop* g_loop;
int g_flag = 0;

void print()
{
    printf("1111111111111111111");
}

void threadFunc()
{
    g_loop->RunEvery(1.0,print);
}

int main()
{
    CEventLoop loop;
    g_loop = &loop;
    CThread t(threadFunc);
    t.Start();

    loop.StartLoop();
}
//int cnt = 0;
//CEventLoop* g_loop;
//
//void printTid()
//{
//    printf("pid = %d, tid = %d\n", getpid(), CurrentThread::Tid());
//    printf("now %s\n", CTimestamp::GetNowTimestamp().ToString().c_str());
//}
//
//void print(const char* msg)
//{
//    printf("msg %s %s\n", CTimestamp::GetNowTimestamp().ToString().c_str(), msg);
//    if (++cnt == 20)
//    {
//        g_loop->QuitLoop();
//    }
//}
//
//int main()
//{
//    printTid();
//    CEventLoop loop;
//    g_loop = &loop;
//
//    print("main");
//    loop.RunAfter(1, std::bind(print, "once1"));
//    loop.RunAfter(1.5, std::bind(print, "once1.5"));
//    loop.RunAfter(2.5, std::bind(print, "once2.5"));
//    loop.RunAfter(3.5, std::bind(print, "once3.5"));
//    loop.RunEvery(2, std::bind(print, "every2"));
//    loop.RunEvery(3, std::bind(print, "every3"));
//
//    loop.StartLoop();
//    print("main loop exits");
//    sleep(1);
//}




