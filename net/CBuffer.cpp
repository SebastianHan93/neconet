//
// Created by hc on 2020/5/12.
//
#include <sys/uio.h>
#include "CBuffer.h"
#include "../base/Types.h"

using namespace neco;
using namespace neco::net;
CBuffer::CBuffer()
    :m_vBuffer(sm_nCheapPrepend+sm_nInitialSize),
     m_nReaderIndex(sm_nCheapPrepend),
     m_nWriterIndex(sm_nCheapPrepend)
{
    assert(GetReadableBytes()==0);
    assert(GetWriteableBytes()==sm_nInitialSize);
    assert(GetPrependableBytes() == sm_nCheapPrepend);
}

void CBuffer::Swap(CBuffer & rhs)
{
    m_vBuffer.swap(rhs.m_vBuffer);
    std::swap(m_nReaderIndex,rhs.m_nReaderIndex);
    std::swap(m_nWriterIndex,rhs.m_nWriterIndex);
}

size_t CBuffer::GetReadableBytes() const
{
    return m_nWriterIndex - m_nReaderIndex;
}

size_t CBuffer::GetWriteableBytes() const
{
    return m_vBuffer.size() - m_nWriterIndex;
}

size_t CBuffer::GetPrependableBytes() const
{
    return m_nReaderIndex;
}

/**
 *
 * @return 返回m_vBuffer可读数据的第一个字节地址
 */
const char * CBuffer::Peek() const
{
    return __Begin() + m_nReaderIndex;
}

/**
 * 如果nLen小于可读长度,则检索到nLen的位置
 * @param nLen
 */
void CBuffer::Retrieve(size_t nLen)
{
    assert(nLen <= GetReadableBytes());
    m_nReaderIndex += nLen;
}

/**
 * 则检索到cEnd的位置 m_nReaderIndex <= cEnd <= m_nWriterIndex
 * @param cEnd
 */
void CBuffer::RetrieveUtil(const char * cEnd)
{
    assert(Peek() <= cEnd);
    assert(cEnd <= BeginWrite());
    Retrieve(cEnd - Peek());
}

/**
 * 返回最初状态
 */
void CBuffer::RetrieveAll()
{
    m_nReaderIndex = sm_nCheapPrepend;
    m_nWriterIndex = sm_nCheapPrepend;
}

/**
 * 将m_nWriterIndex-m_nReaderIndex的数据拿出来返回,并且重置m_nWriterIndex-m_nReaderIndex
 * 也就是返回buffer可读数据并且返回最初状态
 * @return
 */
std::string CBuffer::RetrieveAsString()
{
    std::string sStr(Peek(),GetReadableBytes());
    RetrieveAll();
    return sStr;
}

void CBuffer::Append(const std::string & sStr)
{
    Append(sStr.data(),sStr.length());

}

void CBuffer::Append(const char * pData,size_t nLen)
{
    EnsureWriteableBytes(nLen);
    std::copy(pData,pData+nLen,BeginWrite());
    HasWritten(nLen);
}

void CBuffer::Append(const void* pData,size_t nLen)
{
    Append(static_cast<const char*>(pData),nLen);
}

void CBuffer::EnsureWriteableBytes(size_t nLen)
{
    if(GetWriteableBytes() < nLen)
    {
        __MakeSpace(nLen);
    }
    assert(GetWriteableBytes() >= nLen);
}

char * CBuffer::BeginWrite()
{
    return __Begin() + m_nWriterIndex;
}

const char* CBuffer::BeginWrite() const
{
    return __Begin() + m_nWriterIndex;
}

void CBuffer::HasWritten(size_t nLen)
{
    m_nWriterIndex += nLen;
}

void CBuffer::Prepend(const void * pData,size_t nLen)
{
    assert(nLen<=GetPrependableBytes());
    m_nReaderIndex -= nLen;
    const char * data = static_cast<const char *>(pData);
    std::copy(data,data+nLen,__Begin()+m_nReaderIndex);
}

void CBuffer::Shrink(size_t nReserve)
{
     std::vector<char> vBuf(sm_nCheapPrepend+GetReadableBytes()+nReserve);
     std::copy(Peek(),Peek()+GetReadableBytes(),vBuf.begin()+sm_nCheapPrepend);
     vBuf.swap(m_vBuffer);
}

size_t CBuffer::ReadFd(int nFd,int * pSavedErrno)
{
    char cExtraBuf[65536];
    struct iovec iVec[2];
    const size_t nWriteable = GetWriteableBytes();
    iVec[0].iov_base = __Begin()+m_nWriterIndex;
    iVec[0].iov_len = nWriteable;
    iVec[1].iov_base = cExtraBuf;
    iVec[1].iov_len = sizeof(cExtraBuf);
    const ssize_t n = readv(nFd,iVec,2);
    if(n<0)
    {
        *pSavedErrno = errno;
    }
    else if (implicit_cast<size_t >(n) <= nWriteable)
    {
        m_nWriterIndex+=n;
    }
    else
    {
        m_nWriterIndex = m_vBuffer.size();
        Append(cExtraBuf,n-nWriteable);
    }
    return n;
}

char * CBuffer::__Begin()
{
    return &*m_vBuffer.begin();
}

/**
 *
 * @return 返回m_vBuffer第一个元素字节地址
 */
const char * CBuffer::__Begin() const
{
    return &*m_vBuffer.begin();
}

void CBuffer::__MakeSpace(size_t nLen)
{
    if(GetWriteableBytes() + GetPrependableBytes() < nLen)
    {
        m_vBuffer.resize(m_nWriterIndex + nLen);
    }
    else
    {
        assert(sm_nCheapPrepend < m_nReaderIndex);
        size_t nReadable = GetReadableBytes();
        std::copy(__Begin()+m_nReaderIndex,
                  __Begin()+m_nWriterIndex,
                  __Begin()+sm_nCheapPrepend);
        m_nReaderIndex = sm_nCheapPrepend;
        m_nWriterIndex = m_nReaderIndex + nReadable;
        assert(nReadable == GetReadableBytes());
    }
}