//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CALLBACKS_H
#define NECONET_CALLBACKS_H

#include <functional>

namespace neco
{
    namespace net
    {
        typedef std::function<void()> TIMER_CALL_BACK;
    }
}

#endif //NECONET_CALLBACKS_H
