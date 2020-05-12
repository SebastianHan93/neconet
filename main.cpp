#include <iostream>
#include <map>
#include <stdio.h>
#include "net/CEventLoop.h"
#include "base/CThread.h"
#include "base/CurrentThread.h"
#include "net/CChannel.h"
#include "net/CInetAddress.h"
#include "net/CAcceptor.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include "net/SocketsOps.h"
using namespace neco::net;
using namespace neco;

void newConnection(int sockfd, const CInetAddress& peerAddr)
{
    printf("newConnection(): accepted a new connection from %s\n",
           peerAddr.ToHostPort().c_str());
    ::write(sockfd, "How are you?\n", 13);
    sockets::Close(sockfd);
}

int main()
{
    printf("main(): pid = %d\n", getpid());

    neco::net::CInetAddress listenAddr(10000);
    neco::net::CEventLoop loop;

    neco::net::CAcceptor acceptor(&loop, listenAddr);
    acceptor.SetNewConnectionCallback(newConnection);
    acceptor.StartListen();

    loop.StartLoop();
}
//CEventLoop* g_loop;
//int g_flag = 0;
//
//void print()
//{
//    printf("1111111111111111111");
//}
//
//void threadFunc()
//{
//    g_loop->RunEvery(1.0,print);
//}
//
//int main()
//{
//    CEventLoop loop;
//    g_loop = &loop;
//    CThread t(threadFunc);
//    t.Start();
//
//    loop.StartLoop();
//}
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




