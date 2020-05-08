//
// Created by hc on 2020/5/8.
//

#ifndef NECONET_CPOLLER_H
#define NECONET_CPOLLER_H

#include "../base/noncopyable.h"


namespace neco
{

    namespace net
    {
        class CChannel;
        class CEventLoop;
        class CPoller:noncopyable
        {
        public:
            CPoller(CEventLoop * Eventloop);
//            ~CPoller();

        private:
            CEventLoop * m_pOwnerLoop;
        };
    }
}




#endif //NECONET_CPOLLER_H
