#pragma once

#include "pch.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class Updater
{
private:

	typedef std::vector<float> float_vec;

	float 				m_updateFrequency;

	UINT 				m_nrPointsPerRow;
	UINT 				m_nrPointsPerCol;

	float_vec 			m_points;
	float_vec 			m_workPoints;

	float 				m_min;
	float 				m_max;

	std::mutex			m_mutex;
	std::atomic_bool	m_hasChanged;
	std::atomic_bool 	m_running;
	std::thread			m_workThread;

	void start();
	void stop();

	void run();

public:

	Updater()
	: m_hasChanged(false)
	, m_running(false)
	, m_updateFrequency(0)
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
			memcpy(&m_points[0], &m_workPoints[0], sizeof(float)*m_points.size());
			m_hasChanged = false;
			m_mutex.unlock();
		}

		return &m_points[0];
	}
};
