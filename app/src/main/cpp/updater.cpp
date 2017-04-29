#include "updater.h"

#include <stdlib.h>
#include <unistd.h>
#include <chrono>

bool Updater::init(float updateFrequency, UINT pointsPerRow, UINT pointsPerCol, float base,
				   float interval)
{
	srand((unsigned int)time(nullptr));

	m_updateFrequency = updateFrequency;
	m_nrPointsPerRow = pointsPerRow;
	m_nrPointsPerCol = pointsPerCol;

	m_min = base - interval * 0.5f;
	m_max = base + interval * 0.5f;

	UINT nrPoints = pointsPerRow * pointsPerCol;
	m_points.resize(nrPoints, base);

	for(UINT y = 1; y < m_nrPointsPerCol - 1; ++y)
	{
		for(UINT x = 1; x < m_nrPointsPerRow - 1; ++x)
		{
			m_points[y * m_nrPointsPerRow + x] += static_cast<float>(
					rand() % static_cast<int>(interval * 0.25f)
					- static_cast<int>(interval * 0.125f));
		}
	}

	m_workPoints = float_vec(m_points);

	start();

	return true;
}

void Updater::start()
{
	if(m_running)
		return;

	m_running = true;
	m_workThread = std::thread([=]() { this->run(); });
}

void Updater::stop()
{
	if(!m_running)
		return;

	m_running = false;
	if(m_workThread.joinable())
	{
		try
		{
			m_workThread.join();
		}
		catch(...)
		{
		}
	}
}

void Updater::run()
{
	using framerate = std::chrono::duration<std::chrono::steady_clock::rep, std::ratio<1, 60>>;
	auto next = std::chrono::steady_clock::now() + framerate(1);

	while(m_running)
	{
		std::this_thread::sleep_until(next);
		next += framerate{1};

		for(UINT y = 1; y < m_nrPointsPerCol - 1; ++y)
		{
			for(UINT x = 1; x < m_nrPointsPerRow - 1; ++x)
			{
				UINT index = y * m_nrPointsPerRow + x;
				m_workPoints[index] += static_cast<float>(rand() % 3 - 1);

				m_workPoints[index] = std::min(m_max, std::max(m_min, m_workPoints[index]));
			}
		}

		m_hasChanged = true;
	}
}