//
// Created by hc on 2020/5/11.
//

#ifndef NECONET_SOCKETSOPS_H
#define NECONET_SOCKETSOPS_H

#include <arpa/inet.h>
#include <endian.h>

namespace neco
{
    namespace net
    {
        namespace sockets
        {
            inline u_int64_t HostToNetwork64(uint64_t nHost64)
            {
                return htobe64(nHost64);
            }

            inline u_int32_t HostToNetwork32(uint32_t nHost32)
            {
                return htonl(nHost32);
            }

            inline u_int32_t HostToNetwork16(uint16_t nHost16)
            {
                return htons(nHost16);
            }

            inline u_int64_t NetworkToHost64(uint64_t nNet64)
            {
                return be64toh(nNet64);
            }

            inline u_int32_t NetworkToHost32(uint32_t nNet32)
            {
                return ntohl(nNet32);
            }

            inline u_int16_t NetworkToHost16(uint16_t nNet16)
            {
                return ntohs(nNet16);
            }

            int CreateNonblockingOrDie();
            void BindOrDie(int nSockFd,const struct sockaddr_in & addr);
            void ListenOrDie(int nSockFd);
            int Accept(int nSockFd,struct sockaddr_in* iAddr);
            void Close(int nSockFd);
            void ToHostPort(char * cBuf,size_t nSize,const struct sockaddr_in& iAddr);
            void FromHostPort(const char * pIP,uint16_t nPort, struct sockaddr_in * iAddr);
            struct sockaddr_in GetLocalAddr(int nSockfd);
        }
    }
}

#endif //NECONET_SOCKETSOPS_H
