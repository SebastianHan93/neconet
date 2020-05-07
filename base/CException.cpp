//
// Created by hc on 2020/5/5.
//

#include "CException.h"
#include "CurrentThread.h"
namespace neco
{
    CException::CException(string what)
    :m_sMessage(std::move(what)),m_sStack(CurrentThread::StackTrace(false))
    {

    }

    const char* CException::what() const noexcept
    {
        return m_sMessage.c_str();
    }
    const char* CException::StackTrace() const noexcept
    {
        return m_sStack.c_str();
    }
}
