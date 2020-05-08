//
// Created by hc on 2020/4/29.
//

#ifndef NECONET_NONCOPYABLE_H
#define NECONET_NONCOPYABLE_H
namespace noncopyable_  // protection from unintended ADL
{
    class noncopyable
    {
    public:  // emphasize the following members are private
        noncopyable( const noncopyable& ) = delete;
        const noncopyable& operator=( const noncopyable& )= delete;
    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };
}

typedef noncopyable_::noncopyable noncopyable;

#endif //NECONET_NONCOPYABLE_H
