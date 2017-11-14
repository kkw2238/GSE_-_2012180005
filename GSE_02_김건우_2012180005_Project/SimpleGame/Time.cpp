#include "stdafx.h"
#include "Time.h"


Time::Time() :
	m_dPreframeTime(::timeGetTime()),
	m_dCurframeTime(::timeGetTime()),
	m_fElapsedTime(0.0f),
	m_fTotalframeTime(0.0f),
	m_fTimeScale(0.001f),
	m_ffps(60.0f)
{
}

Time::~Time()
{
}

Time::Time(float fps) :
	m_dPreframeTime(::timeGetTime()),
	m_dCurframeTime(::timeGetTime()),
	m_fElapsedTime(0.0f),
	m_fTotalframeTime(0.0f),
	m_fTimeScale(0.001f),
	m_ffps(fps)
{}

void Time::Update(float fps)
{
	m_ffps = fps;
	m_dCurframeTime = ::timeGetTime();
	
	while ((float)(m_dCurframeTime - m_dPreframeTime) * m_fTimeScale < 1.0f / m_ffps) {
		m_dCurframeTime = ::timeGetTime();
	}
	
	m_fElapsedTime = (m_dCurframeTime - m_dPreframeTime) * m_fTimeScale;
};

std::string Time::GetFrameTime()
{
	std::string sResult;

	if (m_pEverageFrame.size() == MAX_EVERAGE_FRAME) {
		m_fTotalframeTime -= m_pEverageFrame.front();
		m_pEverageFrame.pop();
	}

	m_pEverageFrame.push(m_fElapsedTime);
	m_fTotalframeTime += m_fElapsedTime;

	std::cout << m_fElapsedTime << " " << (int)(1 / (m_fTotalframeTime / m_pEverageFrame.size())) << std::endl;

	sResult = "Game Software Engineering KPU (" + std::to_string((1 / (m_fTotalframeTime / m_pEverageFrame.size()))) + " fps)";
	return sResult;
}
