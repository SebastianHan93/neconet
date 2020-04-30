//
// Created by hc on 2020/4/27.
//

#ifndef NECONET_CTHREAD_H
#define NECONET_CTHREAD_H


#include <functional>
#include <memory>
#include <pthread.h>
#include "noncopyable.h"
#include "CCountDownLatch.h"

using std::string ;
namespace neco{
class CThread :noncopyable{
public:
    typedef std::function<void ()> THREAD_FUNC;

    explicit CThread(THREAD_FUNC, const string& sName = string());
    ~CThread();

public:
    void Start();
    int ThreadJoin();
    bool Started() const ;
    pthread_t GetTid() const ;
    const string& GetName() const;
    static int NumCreated();

private:
    void __SetDefaultName();

private:
    bool m_bStarted;
    bool m_bJoined;
    pid_t m_pthreadId;
    THREAD_FUNC m_pfnFunc;
    string m_sName;
    CCountDownLatch m_clsLatch;
    static AtomicInt32 s_aiNumCreated;

};
}

#endif //NECONET_CTHREAD_H
