//
// Created by hc on 2020/5/11.
//

#ifndef NECONET_CINETADDRESS_H
#define NECONET_CINETADDRESS_H

#include "../base/copyable.h"
#include <netinet/in.h>
#include <string>

namespace neco
{
    namespace net
    {
        class CInetAddress:copyable
        {
        public:
            explicit CInetAddress(uint64_t nPort);
            CInetAddress(const std::string & ip,uint64_t nPort);
            CInetAddress(const struct sockaddr_in& iAddr);

        public:
            std::string ToHostPort() const;
            const struct sockaddr_in & GetSockAddrInet() const;
            void SetSockAddrInet(const struct sockaddr_in& iAddr);

        private:
            struct sockaddr_in m_iAddr;
        };
    }
}



#endif //NECONET_CINETADDRESS_H
