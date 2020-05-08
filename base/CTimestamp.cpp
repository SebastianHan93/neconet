//
// Created by hc on 2020/5/7.
//

#include "CTimestamp.h"
#include <sys/time.h>
#include <stdio.h>
#include <inttypes.h>

using namespace neco;

static_assert(sizeof(CTimestamp) == sizeof(int64_t),
              "Timestamp is same size as int64_t");

const int CTimestamp::sm_nMicroSecondsPerSecond = 1000 * 1000;

CTimestamp::CTimestamp():m_nMicroSecondsSinceEpoch(0)
{

}

CTimestamp::CTimestamp(int64_t nMicroSecondsSinceEpochArg):m_nMicroSecondsSinceEpoch(nMicroSecondsSinceEpochArg)
{

}

void CTimestamp::Swap(CTimestamp& iTimestamp)
{
    std::swap(m_nMicroSecondsSinceEpoch,iTimestamp.m_nMicroSecondsSinceEpoch);
}

string CTimestamp::ToString() const
{
    char cBuf[32] = {0};
    int64_t nSeconds = m_nMicroSecondsSinceEpoch/sm_nMicroSecondsPerSecond;
    int64_t nMicroseconds = m_nMicroSecondsSinceEpoch % sm_nMicroSecondsPerSecond;
    snprintf(cBuf, sizeof(cBuf)-1,"%" PRId64 ".%06" PRId64 "",nSeconds,nMicroseconds);
    return cBuf;
}

string CTimestamp::ToFormattedString(bool bShowMicroseconds) const
{
    char cBuf[64] = {0};
    time_t tSeconds = static_cast<time_t >(m_nMicroSecondsSinceEpoch/sm_nMicroSecondsPerSecond);
    struct tm tmTime;
    gmtime_r(&tSeconds,&tmTime);
    if(bShowMicroseconds)
    {
        int nMicroseconds = static_cast<int>(m_nMicroSecondsSinceEpoch%sm_nMicroSecondsPerSecond);
        snprintf(cBuf, sizeof(cBuf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
                 tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
                 nMicroseconds);
    }
    else
    {
        snprintf(cBuf, sizeof(cBuf), "%4d%02d%02d %02d:%02d:%02d",
                 tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
                 tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
    }
    return cBuf;
}

bool CTimestamp::IsValid() const
{
    return m_nMicroSecondsSinceEpoch>0;
}

int64_t CTimestamp::GetMicroSecondsSinceEpoch() const
{
    return m_nMicroSecondsSinceEpoch;
}

time_t CTimestamp::GetSecondsSinceEpoch() const
{
    return static_cast<time_t >(m_nMicroSecondsSinceEpoch/sm_nMicroSecondsPerSecond);
}

CTimestamp CTimestamp::GetNowTimestamp()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    int64_t nSeconds = tv.tv_sec;
    return CTimestamp(nSeconds*sm_nMicroSecondsPerSecond+tv.tv_usec);
}

CTimestamp CTimestamp::Invalid()
{
    return CTimestamp();
}

CTimestamp CTimestamp::FromUnixTime(time_t t)
{
    return FromUnixTime(t,0);
}

CTimestamp CTimestamp::FromUnixTime(time_t t,int nMicroseconds)
{
    return CTimestamp(static_cast<int64_t >(t)*sm_nMicroSecondsPerSecond+nMicroseconds);
}

