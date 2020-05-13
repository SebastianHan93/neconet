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
#include "net/CTcpConnection.h"
#include "net/CTcpServer.h"
using namespace neco::net;
using namespace neco;


void onConnection(const C_TCP_CONNECTION_PTR & conn)
{
    if (conn->IsConnected())
    {
        printf("onConnection(): new connection [%s] from %s\n",
               conn->GetName().c_str(),
               conn->GetPeerAddress().ToHostPort().c_str());
    }
    else
    {
        printf("onConnection(): connection [%s] is down\n",
               conn->GetName().c_str());
    }
}

void onMessage(const C_TCP_CONNECTION_PTR& conn,
               CBuffer *buf,
               CTimestamp timestamp)
{
    printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
           buf->GetReadableBytes(),
           conn->GetName().c_str(),
           timestamp.ToFormattedString().c_str());
    printf("onMessage(): [%s]\n", buf->RetrieveAsString().c_str());

}

int main()
{
    printf("main(): pid = %d\n", getpid());

    CInetAddress listenAddr(9981);
    CEventLoop loop;

    CTcpServer server(&loop, listenAddr);
    server.SetConnectionCallback(onConnection);
    server.SetMessageCallback(onMessage);
    server.Start();

    loop.StartLoop();
}