//
// Created by hc on 2020/5/12.
//

#ifndef NECONET_CBUFFER_H
#define NECONET_CBUFFER_H

#include <algorithm>
#include <string>
#include <vector>
#include <assert.h>
#include "../base/noncopyable.h"

namespace neco
{
    namespace net
    {
        class CBuffer : noncopyable{
        public:
            CBuffer();
        public:
            void Swap(CBuffer & rhs);
            size_t GetReadableBytes() const;
            size_t GetWriteableBytes() const;
            size_t GetPrependableBytes() const;
            const char * Peek() const;
            void Retrieve(size_t nLen);
            void RetrieveUtil(const char * cEnd);
            void RetrieveAll();
            std::string RetrieveAsString();
            void Append(const std::string & sStr);
            void Append(const char * pData,size_t nLen);
            void Append(const void* pData,size_t nLen);
            void EnsureWriteableBytes(size_t nLen);
            char * BeginWrite();
            const char* BeginWrite() const;
            void HasWritten(size_t nLen);
            void Prepend(const void * pData,size_t nLen);
            void Shrink(size_t nReserve);
            size_t ReadFd(int nFd,int * pSavedErrno);
        public:
            static const size_t sm_nCheapPrepend = 8;
            static const size_t sm_nInitialSize = 1024;
        private:
            char * __Begin();
            const char * __Begin() const;
            void __MakeSpace(size_t nLen);
        private:
            std::vector<char > m_vBuffer;
            size_t m_nReaderIndex;
            size_t m_nWriterIndex;
        };
    }
}



#endif //NECONET_CBUFFER_H
