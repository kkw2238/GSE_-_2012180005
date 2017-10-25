#pragma once
#include "stdafx.h"

class Time
{
private:
	double m_dPreframeTime;
	double m_dCurframeTime;
	float  m_ffps;
public:
	Time();
	~Time();

public:
	void Update(float fps) { 
		m_ffps = fps; 
		while ((float)(::timeGetTime() - m_dPreframeTime) / 1000 < 1 / m_ffps) {}
		m_dCurframeTime = ::timeGetTime();
	};

	float Tick() const { return (float)(m_dCurframeTime - m_dPreframeTime) / 1000; }
	void Tock() { m_dPreframeTime = m_dCurframeTime; }
};

