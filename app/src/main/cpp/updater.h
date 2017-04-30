#pragma once

#include "pch.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class Updater
{
private:

	struct Point
	{
		float force;
	};

	struct Impact
	{
		enum TYPE { TOUCH };

		TYPE type;
		UINT row;
		UINT col;
	};

	typedef std::vector<Point> point_vec;
	typedef std::vector<Impact> impact_vec;
	typedef std::vector<float> float_vec;

	float 				m_updateFrequency;

	UINT 				m_nrPointsPerRow;
	UINT 				m_nrPointsPerCol;

	impact_vec			m_impacts;

	point_vec			m_points;
	float_vec 			m_values;
	float_vec 			m_workValues;

	float 				m_min;
	float 				m_max;
	float 				m_base;

	float 				m_inverseForceFactor;
	float 				m_gravityFactor;
	float 				m_perTouchFactor;

	std::mutex			m_mutex;
	std::atomic_bool	m_hasChanged;
	std::atomic_bool 	m_running;
	std::thread			m_workThread;

	void start();
	void stop();

	void run();

	void handleImpacts();
	void applyForces(float dt);
	void addForce(int row, int col, int modRow, int modCol, float force);

public:

	Updater()
	: m_hasChanged(false)
	, m_running(false)
	, m_updateFrequency(0)
	, m_min(0)
	, m_max(0)
	, m_base(0)
	, m_inverseForceFactor(0.55f)
	, m_gravityFactor(0.85f)
	, m_perTouchFactor(5.0f)
	{
	}

	virtual ~Updater()
	{
		stop();
	}

	bool init(float updateFrequency, UINT pointsPerRow, UINT pointsPerCol, float base,
			  float interval);

	const float* const getPoints()
	{
		if(m_hasChanged)
		{
			m_mutex.lock();
			memcpy(&m_values[0], &m_workValues[0], sizeof(float)*m_values.size());
			m_hasChanged = false;
			m_mutex.unlock();
		}

		return &m_values[0];
	}

	void touch(UINT row, UINT col);
};
