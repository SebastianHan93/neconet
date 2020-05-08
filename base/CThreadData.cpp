//
// Created by hc on 2020/5/5.
//
#include <sys/prctl.h>
#include <exception>
#include "CThreadData.h"
#include "CurrentThread.h"
#include "CException.h"
#include <sys/syscall.h>
#include <unistd.h>

namespace neco
{
    namespace detail
    {
        CThreadData::CThreadData(THREAD_FUNC pfnThreadFunc,const string& sName,pid_t* ThreadTid,CCountDownLatch *clsLatch)
        :m_pfnThreadFunc(std::move(pfnThreadFunc)),m_sName(sName),m_ThreadTid(ThreadTid),m_clsLatch(clsLatch)
        {

        }

        void CThreadData::RunInThread()
        {
            *m_ThreadTid = neco::CurrentThread::Tid();
            m_ThreadTid = nullptr;
            m_clsLatch->CountDown();
            m_clsLatch = nullptr;

            neco::CurrentThread::t_pcThreadName = m_sName.empty() ? "necoThread" : m_sName.c_str();
            ::prctl(PR_SET_NAME,neco::CurrentThread::t_pcThreadName);
            try
            {
                m_pfnThreadFunc();
                neco::CurrentThread::t_pcThreadName="finished";
            }
            catch (const CException& exception)
            {
                neco::CurrentThread::t_pcThreadName = "crashed";
                fprintf(stderr, "exception caught in Thread %s\n", m_sName.c_str());
                fprintf(stderr, "reason: %s\n", exception.what());
                fprintf(stderr, "stack trace: %s\n", exception.StackTrace());
                abort();
            }
            catch (const std::exception& exception)
            {
                neco::CurrentThread::t_pcThreadName = "crashed";
                fprintf(stderr, "exception caught in Thread %s\n", m_sName.c_str());
                fprintf(stderr, "reason: %s\n", exception.what());
                abort();
            }
            catch (...)
            {
                neco::CurrentThread::t_pcThreadName = "crashed";
                fprintf(stderr, "unknown exception caught in Thread %s\n", m_sName.c_str());
                throw; // rethrow
            }
        }

        void* StartThread(void* obj)
        {
            CThreadData* data = static_cast<CThreadData*>(obj);
            data->RunInThread();
            delete data;
            return NULL;
        }

        pid_t GetTid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

        void AfterFork()
        {
            neco::CurrentThread::t_nCachedTid = 0;
            neco::CurrentThread::t_pcThreadName = "main";
            CurrentThread::Tid();
            // no need to call pthread_atfork(NULL, NULL, &afterFork);
        }

        CThreadNameInitializer::CThreadNameInitializer()
        {

            neco::CurrentThread::t_pcThreadName = "main";
            CurrentThread::Tid();
            pthread_atfork(NULL, NULL, &AfterFork);

        }
    }
}