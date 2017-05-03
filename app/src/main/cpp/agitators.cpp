#include <cmath>
#include "agitators.h"
#include "stdlib.h"

void Wave::run(Updater* pUpdater, float dt)
{
	m_timer += m_speed * dt;

	for(UINT x = 1; x < pUpdater->getNrPointsPerRow()-1; ++x)
	{
		float f = std::sin( m_timer + (static_cast<float>(x) * m_separation));
		if(f > 0.8f)
		{
			for(UINT y=1; y<pUpdater->getNrPointsPerCol()-1; ++y)
			{
				pUpdater->addForce(y,x, m_influence * f * dt);
			}
		}
	}
}

void Rain::run(Updater* pUpdater, float dt)
{
	m_timer += dt;
	if(m_timer > m_interval)
	{
		m_timer = 0;
		UINT row = rand() % (pUpdater->getNrPointsPerCol() - 2) + 1;
		UINT col = rand() % (pUpdater->getNrPointsPerRow() - 2) + 1;

		pUpdater->addForce(row, col, 0, 0, m_strength);
	}
}