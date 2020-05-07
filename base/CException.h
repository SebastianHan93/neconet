//
// Created by hc on 2020/5/5.
//

#ifndef NECONET_CEXCEPTION_H
#define NECONET_CEXCEPTION_H

#include <bits/exception.h>
#include "Types.h"

namespace neco
{
class CException :public std::exception{

public:
    CException(string what);
    ~CException() noexcept override = default;

public:
    const char* what() const noexcept override;
    const char* StackTrace() const noexcept ;


private:
    string m_sMessage;
    string m_sStack;
};
}



#endif //NECONET_CEXCEPTION_H
