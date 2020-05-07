//
// Created by hc on 2020/5/5.
//

#ifndef NECONET_CTHREADDATA_H
#define NECONET_CTHREADDATA_H

#include "CThread.h"

namespace neco
{
    namespace detail
    {
        struct CThreadData{
            typedef neco::CThread::THREAD_FUNC THREAD_FUNC;
            THREAD_FUNC m_pfnThreadFunc;
            string m_sName;
            pid_t * m_ThreadTid;
            CCountDownLatch* m_clsLatch;

            CThreadData(THREAD_FUNC pfnThreadFunc,
                    const string& sName,
                    pid_t* ThreadTid,
                    CCountDownLatch *clsLatch);
            void RunInThread();
        };

        void* StartThread(void* obj);
    }
}



#endif //NECONET_CTHREADDATA_H
