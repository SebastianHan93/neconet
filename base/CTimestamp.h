//
// Created by hc on 2020/5/7.
//

#ifndef NECONET_CTIMESTAMP_H
#define NECONET_CTIMESTAMP_H

#include "copyable.h"
#include "Types.h"

namespace neco
{
    class CTimestamp : public copyable
    {
    public:
        CTimestamp();
        explicit CTimestamp(int64_t nMicroSecondsSinceEpochArg);

    public:
        void Swap(CTimestamp& iTimestamp);
        string ToString() const;
        string ToFormattedString(bool bShowMicroseconds= true) const;
        bool IsValid() const;
        int64_t GetMicroSecondsSinceEpoch() const;
        time_t GetSecondsSinceEpoch() const;
        static CTimestamp GetNowTimestamp();
        static CTimestamp Invalid();
        static CTimestamp FromUnixTime(time_t t);
        static CTimestamp FromUnixTime(time_t t,int nMicroseconds);
        friend bool operator<(CTimestamp lhs, CTimestamp rhs);
        friend bool operator>(CTimestamp lhs, CTimestamp rhs);
        friend bool operator<=(CTimestamp lhs, CTimestamp rhs);
        friend bool operator>=(CTimestamp lhs, CTimestamp rhs);
        friend bool operator==(CTimestamp lhs, CTimestamp rhs);
        friend CTimestamp AddTime(CTimestamp timestamp, double seconds);
        friend double TimeDifference(CTimestamp high, CTimestamp low);
    public:
        static const int sm_nMicroSecondsPerSecond;

    private:
        int64_t m_nMicroSecondsSinceEpoch;
    };

    inline bool operator<(CTimestamp lhs, CTimestamp rhs)
    {
        return static_cast<bool>(lhs.GetMicroSecondsSinceEpoch() < rhs.GetMicroSecondsSinceEpoch());
    }

    inline bool operator>(CTimestamp lhs, CTimestamp rhs)
    {
        return static_cast<bool>(lhs.GetMicroSecondsSinceEpoch() > rhs.GetMicroSecondsSinceEpoch());
    }

    inline bool operator<=(CTimestamp lhs, CTimestamp rhs)
    {
        return static_cast<bool>(lhs.GetMicroSecondsSinceEpoch() <= rhs.GetMicroSecondsSinceEpoch());
    }

    inline bool operator>=(CTimestamp lhs, CTimestamp rhs)
    {
        return static_cast<bool>(lhs.GetMicroSecondsSinceEpoch() >= rhs.GetMicroSecondsSinceEpoch());
    }

    inline bool operator==(CTimestamp lhs, CTimestamp rhs)
    {
        return lhs.GetMicroSecondsSinceEpoch() == rhs.GetMicroSecondsSinceEpoch();
    }

    inline double TimeDifference(CTimestamp high, CTimestamp low)
    {
        int64_t diff = high.GetMicroSecondsSinceEpoch() - low.GetMicroSecondsSinceEpoch();
        return static_cast<double>(diff) / CTimestamp::sm_nMicroSecondsPerSecond;
    }

    inline CTimestamp AddTime(CTimestamp timestamp, double seconds)
    {
        int64_t delta = static_cast<int64_t>(seconds * CTimestamp::sm_nMicroSecondsPerSecond);
        return CTimestamp(timestamp.GetMicroSecondsSinceEpoch() + delta);
    }
}



#endif //NECONET_CTIMESTAMP_H
