//
// Created by hc on 2020/4/29.
//

#ifndef NECONET_NONCOPYABLE_H
#define NECONET_NONCOPYABLE_H
namespace noncopyable_  // protection from unintended ADL
{
    class noncopyable
    {
    protected:
        noncopyable() {}
        ~noncopyable() {}
    private:  // emphasize the following members are private
        noncopyable( const noncopyable& );
        const noncopyable& operator=( const noncopyable& );
    };
}

typedef noncopyable_::noncopyable noncopyable;

#endif //NECONET_NONCOPYABLE_H
