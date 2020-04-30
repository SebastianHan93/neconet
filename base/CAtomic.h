//
// Created by hc on 2020/4/30.
//

#ifndef NECONET_CATOMIC_H
#define NECONET_CATOMIC_H

#include <stdint.h>
#include "noncopyable.h"

namespace neco
{
    namespace detail
    {
        template <typename T>
        class CAtomicIntegerT :noncopyable{
        public:
            CAtomicIntegerT():m_value(0){}

            T Get()
            {
                return __sync_val_compare_and_swap(&m_value,0,0);
            }

            T GetAndAdd(T x)
            {
                return __sync_fetch_and_add(&m_value,x);
            }

            T AddAndGet(T x)
            {
                return GetAndAdd(x) + x;
            }

            T IncrementAndGet()
            {
                return AddAndGet(1);
            }

            T DecrementAndGet()
            {
                return AddAndGet(-1);
            }

            void Add(T x)
            {
                GetAndAdd(x);
            }

            void Increment()
            {
                IncrementAndGet();
            }

            void Decrement()
            {
                DecrementAndGet();
            }

            T GetAndSet(T newValue)
            {
                return __sync_lock_test_and_set(&m_value, newValue);
            }

        private:
            volatile T m_value;
        };
    }
    typedef detail::CAtomicIntegerT<int32_t> AtomicInt32;
    typedef detail::CAtomicIntegerT<int64_t> AtomicInt64;
}



#endif //NECONET_CATOMIC_H
