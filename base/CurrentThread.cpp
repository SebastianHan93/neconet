//
// Created by hc on 2020/4/30.
//
#include <errno.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>
#include <linux/unistd.h>

#include "CCondition.h"
#include "CurrentThread.h"
#include "CTimestamp.h"
#include "CThreadData.h"
#include <zconf.h>

namespace neco
{
    namespace CurrentThread
    {
        __thread int t_nCachedTid = 0;
        __thread char t_gcTidString[32];
        __thread int t_nTidStringLength = 6;
        __thread const char * t_pcThreadName = "unknown";
        static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");
        string StackTrace(bool bDemangle)
        {
            string stack;
            const int max_frames = 200;
            void* frame[max_frames];
            int nptrs = ::backtrace(frame, max_frames);
            char** strings = ::backtrace_symbols(frame, nptrs);
            if (strings)
            {
                size_t len = 256;
                char* demangled = bDemangle ? static_cast<char*>(::malloc(len)) : nullptr;
                for (int i = 1; i < nptrs; ++i)  // skipping the 0-th, which is this function
                {
                    if (bDemangle)
                    {
                        // https://panthema.net/2008/0901-stacktrace-demangled/
                        // bin/exception_test(_ZN3Bar4testEv+0x79) [0x401909]
                        char* left_par = nullptr;
                        char* plus = nullptr;
                        for (char* p = strings[i]; *p; ++p)
                        {
                            if (*p == '(')
                                left_par = p;
                            else if (*p == '+')
                                plus = p;
                        }

                        if (left_par && plus)
                        {
                            *plus = '\0';
                            int status = 0;
                            char* ret = abi::__cxa_demangle(left_par+1, demangled, &len, &status);
                            *plus = '+';
                            if (status == 0)
                            {
                                demangled = ret;  // ret could be realloc()
                                stack.append(strings[i], left_par+1);
                                stack.append(demangled);
                                stack.append(plus);
                                stack.push_back('\n');
                                continue;
                            }
                        }
                    }
                    // Fallback to mangled names
                    stack.append(strings[i]);
                    stack.push_back('\n');
                }
                free(demangled);
                free(strings);
            }
            return stack;
        }
        bool IsMainThread()
        {
            return Tid() == ::getpid();
        }

        void SleepUsec(int64_t i64Usec)
        {
            struct timespec ts = {0,0};
            ts.tv_sec = static_cast<time_t>(i64Usec/CTimestamp::sm_nMicroSecondsPerSecond);
            ts.tv_nsec = static_cast<long>(i64Usec%CTimestamp::sm_nMicroSecondsPerSecond);
            ::nanosleep(&ts, NULL);
        }

        void CacheTid()
        {
            if(t_nCachedTid == 0)
            {
                t_nCachedTid = detail::GetTid();
                t_nTidStringLength = snprintf(t_gcTidString,sizeof t_gcTidString,"%5d",t_nCachedTid);
            }
        }

    }
}
