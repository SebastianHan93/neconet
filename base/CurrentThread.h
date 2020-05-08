//
// Created by hc on 2020/4/29.
//

#ifndef NECONET_CURRENTTHREAD_H
#define NECONET_CURRENTTHREAD_H

#include <string>
#include <stdint-gcc.h>
#include "Types.h"

namespace neco
{
    namespace CurrentThread
    {
        //__thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。__thread变量每一个线程有一份独立实体，各个线程的值互不干扰。可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。
        //__thread使用规则：只能修饰POD类型(类似整型指针的标量，不带自定义的构造、拷贝、赋值、析构的类型，二进制内容可以任意复制memset,memcpy,且内容可以复原)，不能修饰class类型，因为无法自动调用构造函数和析构函数，可以用于修饰全局变量，函数内的静态变量，不能修饰函数的局部变量或者class的普通成员变量，且__thread变量值只能初始化为编译器常量(值在编译器就可以确定const int i=5,运行期常量是运行初始化后不再改变const int i=rand()).
//        using std::string;
        extern __thread int t_nCachedTid;
        extern __thread char t_gcTidString[32];
        extern __thread int t_nTidStringLength;
        extern __thread const char * t_pcThreadName;

        void CacheTid();
        /**
         * 暂时不知道这玩意tm的是干嘛的
         * @return
         */
        inline int Tid()
        {
//        __builtin_expect() 是 GCC (version >= 2.96）提供给程序员使用的，目的是将“分支转移”的信息提供给编译器，这样编译器可以对代码进行优化，以减少指令跳转带来的性能下降。
//        __builtin_expect((x),1)表示 x 的值为真的可能性更大；
//        __builtin_expect((x),0)表示 x 的值为假的可能性更大。
//        也就是说，使用likely()，执行 if 后面的语句的机会更大，使用 unlikely()，执行 else 后面的语句的机会更大。通过这种方式，编译器在编译过程中，会将可能性更大的代码紧跟着起面的代码，从而减少指令跳转带来的性能上的下降。
            //期望t_iCachedTid=0 ==0的可能性很小
            if(__builtin_expect(t_nCachedTid == 0,0))
            {
                CacheTid();
            }
            return t_nCachedTid;
        }

        inline const char * GeTTidString()// for logging
        {
            return t_gcTidString;
        }

        inline int GETTidStringLength()// for logging
        {
            return t_nTidStringLength;
        }

        inline const char * name()
        {
            return t_pcThreadName;
        }

        bool IsMainThread();

        void SleepUsec(int64_t i64Usec);

        string StackTrace(bool bDemangle);

    }
}

#endif //NECONET_CURRENTTHREAD_H
