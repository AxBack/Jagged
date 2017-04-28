#include "updater.h"

#include <stdlib.h>
#include <time.h>

bool Updater::init(float updateFrequency, UINT pointsPerRow, UINT pointsPerCol, float base,
				   float interval)
{
	srand((unsigned int)time(nullptr));

	m_updateFrequencey = updateFrequency;
	m_nrPointsPerRow = pointsPerRow;
	m_nrPointsPerCol = pointsPerCol;

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
	return true;
}