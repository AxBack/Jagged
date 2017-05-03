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

	float m_separation;

public:

	Wave(float influence, float speed, float separation)
			: m_influence(influence), m_speed(speed), m_timer(0), m_separation(separation)
	{
	}

	virtual void run(Updater* pUpdater, float dt) override;

};

class Rain : public Agitator
{
private:

	float m_strength;
	float m_interval;
	float m_timer;

public:

	Rain(float strength, float interval)
			: m_strength(strength), m_interval(interval), m_timer(0.0f)
	{
	}

	virtual void run(Updater* pUpdater, float dt) override;
};

#include "updater.h"
