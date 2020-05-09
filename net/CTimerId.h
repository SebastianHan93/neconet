//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CTIMERID_H
#define NECONET_CTIMERID_H

#include "../base/copyable.h"

namespace neco
{
    namespace net
    {
        class CTimer;
        class CTimerId:copyable
        {
        public:
            explicit CTimerId(CTimer* iTimer);

        private:
            CTimer * m_iTimer;
        };
    }
}



#endif //NECONET_CTIMERID_H
