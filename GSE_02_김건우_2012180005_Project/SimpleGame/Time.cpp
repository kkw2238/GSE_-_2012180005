#include "stdafx.h"
#include "Time.h"


Time::Time() :
	m_dPreframeTime(::timeGetTime()),
	m_dCurframeTime(::timeGetTime())
{
}


Time::~Time()
{
}
