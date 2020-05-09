//
// Created by hc on 2020/5/8.
//

#include "CTimer.h"
using namespace neco;
using namespace neco::net;

CTimer::CTimer(TIMER_CALL_BACK cb,CTimestamp iWhen, double dfInterval)
    :m_ifnCallback(std::move(cb)),m_iExpiration(iWhen),m_dfInterval(dfInterval),m_bRepeat(dfInterval>0.0)
{

}
void CTimer::Run() const
{
    m_ifnCallback();
}

CTimestamp CTimer::GetExpiration() const
{
    return m_iExpiration;
}

bool CTimer::GetRepeat()
{
    return m_bRepeat;
}

void CTimer::Restart(CTimestamp iNowTime)
{
    if(m_bRepeat)
    {
        m_iExpiration = AddTime(iNowTime,m_dfInterval);
    }
    else
    {
        m_iExpiration = CTimestamp::Invalid();
    }
}