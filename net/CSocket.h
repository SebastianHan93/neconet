//
// Created by hc on 2020/5/11.
//

#ifndef NECONET_CSOCKET_H
#define NECONET_CSOCKET_H

#include "../base/noncopyable.h"

namespace neco
{
    namespace net
    {
        class CInetAddress;
        class CSocket:noncopyable
        {
        public:
            explicit CSocket(int nSockFd);
            ~CSocket();

        public:
            int GetSocketFd() const;
            void BindAddredd(const CInetAddress& iLocalAddr);
            void Listen();
            int Accept(CInetAddress * iPeerAddr);
            void SetReuseAddr(bool bOn);

        private:
            const int m_nSockFd;
        };
    }
}



#endif //NECONET_CSOCKET_H
