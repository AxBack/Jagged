#pragma once

class Updater;

class Agitator
{
public:

	virtual~ Agitator()
	{}

	virtual void run(Updater* pUpdater, float dt) = 0;
};

class Wave : public Agitator
{
private:

	float m_influence;

	float m_timer;
	float m_speed;

	float m_min;
	float m_max;

public:

	Wave(float influence, float speed, float min, float max)
			: m_influence(influence)
			, m_speed(speed)
			, m_timer(0)
			, m_min(min)
			, m_max(max)
	{
	}

	virtual void run(Updater* pUpdater, float dt) override;

};

#include "updater.h"
