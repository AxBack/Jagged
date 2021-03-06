#include "updater.h"

#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include <cmath>

#define EPSILON 0.0001f

bool Updater::init(float updateFrequency, UINT pointsPerRow, UINT pointsPerCol, float base,
				   float interval, float maxDiffX, float maxDiffY,
				   UINT numAgitators, Agitator** ppAgitators)
{
	stop();
	clearAgitators();

	srand((unsigned int)time(nullptr));

	std::lock_guard<std::mutex> _(m_mutex);

	m_updateFrequency = updateFrequency;
	m_nrPointsPerRow = pointsPerRow;
	m_nrPointsPerCol = pointsPerCol;

	m_base = base;
	m_min = base - interval * 0.5f;
	m_max = base + interval * 0.5f;

	m_maxDiffX = maxDiffX;
	m_maxDiffY = maxDiffY;

	UINT nrPoints = pointsPerRow * pointsPerCol;
	m_points.resize(nrPoints, {0});
	m_values.resize(nrPoints, base);
	m_workValues = float_vec(m_values);

	if(numAgitators > 0)
		add(numAgitators, ppAgitators);

	return true;
}

void Updater::clearAgitators()
{
	for(auto& it : m_agitatorOffloader)
		SAFE_DELETE(it);
	m_agitatorOffloader.clear();

	for(auto& it : m_agitators)
		SAFE_DELETE(it);
	m_agitators.clear();
}

void Updater::touch(UINT row, UINT col)
{
	if(row < m_nrPointsPerCol && col < m_nrPointsPerRow)
	{
		std::lock_guard<std::mutex> _(m_mutex);
		m_touches.push_back({row, col});
	}
}

void Updater::add(const int numAgitators, Agitator** ppAgitators)
{
	for(UINT i = 0; i < numAgitators; ++i)
		m_agitatorOffloader.push_back(ppAgitators[i]);
}

void Updater::start()
{
	std::lock_guard<std::mutex> _(m_mutex);
	if(m_running)
		return;

	m_running = true;
	m_workThread = std::thread([=]() { this->run(); });
}

void Updater::stop()
{
	if(!m_running)
		return;

	{
		std::lock_guard<std::mutex> _(m_mutex);
		m_running = false;
	}

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

	while(true)
	{
		std::this_thread::sleep_until(next);
		next += framerate{1};

		handleImpacts();
		evenOut(m_updateFrequency);

		std::lock_guard<std::mutex> _(m_mutex);
		if(!m_running)
			break;

		if(m_agitatorOffloader.size() > 0)
		{
			m_agitators.insert(m_agitators.end(), m_agitatorOffloader.begin(),
							   m_agitatorOffloader.end());
			m_agitatorOffloader.clear();
		}

		applyForces(m_updateFrequency);
	}
}

void Updater::handleImpacts()
{
	for(auto& it : m_touches)
		addForce(it.row, it.col, 0, 0, m_perTouchFactor);
	m_touches.clear();

	for(auto& it : m_agitators)
		it->run(this, m_updateFrequency);
}

void Updater::applyForces(float dt)
{
	float interval = (m_max - m_min) * 0.5f;

	bool changed = false;
	for(UINT index = 0; index < m_points.size(); ++index)
	{
		if(std::abs(m_points[index].force) > EPSILON)
		{
			m_workValues[index] = std::min(m_max,
										   std::max(m_min, m_workValues[index] +
														   m_points[index].force * dt));
			changed = true;
		}

		float diff = m_workValues[index] - m_base;
		m_points[index].force -= ((std::abs(diff) / interval) * m_points[index].force * dt);

		if(std::abs(diff) > EPSILON)
			m_points[index].force -= (diff * m_gravityFactor * dt);
	}

	if(changed)
		m_hasChanged = true;
}

void Updater::evenOut(float dt)
{
	for(UINT row = 1; row < m_nrPointsPerCol - 1; ++row)
	{
		for(UINT col = 1; col < m_nrPointsPerRow - 1; ++col)
		{
			unsigned int index = row * m_nrPointsPerRow + col;
			if(col < m_nrPointsPerRow - 3)
				evenOut(index, index + 1, m_maxDiffX, dt);

			if(row < m_nrPointsPerCol - 3)
				evenOut(index, index + m_nrPointsPerRow, m_maxDiffY, dt);
		}
	}
}

void Updater::evenOut(UINT p1, UINT p2, float maxDiff, float dt)
{
	float diff = m_workValues[p1] - m_workValues[p2];
	if(std::abs(diff) > maxDiff)
	{
		m_points[p1].force -= diff * dt;
		m_points[p2].force += diff * dt;
	}
}

void Updater::addForce(int row, int col, float force)
{
	m_points[row * m_nrPointsPerRow + col].force += force;
}

void Updater::addForce(int row, int col, int modRow, int modCol, float force)
{
	if(std::abs(force) < 1.0f
	   || row == 0 || row == m_nrPointsPerCol - 1
	   || col == 0 || col == m_nrPointsPerRow - 1)
		return;

	m_points[row * m_nrPointsPerRow + col].force += force;

	if(modRow <= 0)
		addForce(row - 1, col, -1, 0, force * 0.5f);

	if(modRow >= 0)
		addForce(row + 1, col, 1, 0, force * 0.5f);

	if(modCol <= 0)
		addForce(row, col - 1, 0, -1, force * 0.5f);

	if(modCol >= 0)
		addForce(row, col + 1, 0, 1, force * 0.5f);
}