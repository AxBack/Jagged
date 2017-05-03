#pragma once

#include "pch.h"
#include "agitators.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "agitators.h"

class Updater
{
public:

	struct Point
	{
		float x, y;
		float force;
	};

private:

	struct Touch
	{
		UINT row;
		UINT col;
	};

	typedef std::vector<Agitator*> agitator_vec;
	typedef std::vector<Point> point_vec;
	typedef std::vector<Touch> touch_vec;
	typedef std::vector<float> float_vec;

	float 				m_updateFrequency;

	UINT 				m_nrPointsPerRow;
	UINT 				m_nrPointsPerCol;

	agitator_vec		m_agitatorOffloader;

	agitator_vec		m_agitators;
	touch_vec			m_touches;

	point_vec			m_points;
	float_vec 			m_values;
	float_vec 			m_workValues;

	float 				m_min;
	float 				m_max;
	float 				m_base;

	float 				m_maxDiffX;
	float 				m_maxDiffY;

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
	void evenOut(float dt);
	void evenOut(UINT p1, UINT p2, float maxDiff, float dt);

	void clearAgitators(bool lock = true);
	void add(const int numAgitators, Agitator** ppAgitators);

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
	, m_maxDiffX(100.0f)
	, m_maxDiffY(100.0f)
	{
	}

	virtual ~Updater()
	{
		stop();
		clearAgitators(false);
	}

	bool init(float updateFrequency, UINT pointsPerRow, UINT pointsPerCol, float base,
			  float interval, float maxDiffX, float maxDiffY,
			  UINT numAgitators, Agitator** ppAgitators);

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

	void addForce(int row, int col, float force);
	void addForce(int row, int col, int modRow, int modCol, float force);

	UINT getNrPointsPerRow() const { return m_nrPointsPerRow; }
	UINT getNrPointsPerCol() const { return m_nrPointsPerCol; }
};
