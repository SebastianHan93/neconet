//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CALLBACKS_H
#define NECONET_CALLBACKS_H

#include <functional>
#include <memory>
#include "../base/CTimestamp.h"

namespace neco
{
    template<typename T>
    inline T* get_pointer(const std::unique_ptr<T>& ptr)
    {
        return ptr.get();
    }

    namespace net
    {
        class CBuffer;
        class CTcpConnection;
        typedef std::shared_ptr<CTcpConnection> C_TCP_CONNECTION_PTR;

        typedef std::function<void()> TIMER_CALL_BACK;
        typedef std::function<void (const C_TCP_CONNECTION_PTR&)> CONNECTION_CALLBACK;
        typedef std::function<void (const C_TCP_CONNECTION_PTR&,CBuffer *,CTimestamp)> MESSAGE_CALLBACK;
        typedef std::function<void (const C_TCP_CONNECTION_PTR&)> CLOSE_CALLBACK;

//        typedef std::function<void (const C_TCP_CONNECTION_PTR&)> WriteCompleteCallback;
//        typedef std::function<void (const C_TCP_CONNECTION_PTR&, size_t)> HighWaterMarkCallback;

    }
}

#endif //NECONET_CALLBACKS_H
