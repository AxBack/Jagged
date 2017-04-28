#pragma once

#include "pch.h"
#include <vector>

class Updater
{
private:

	typedef std::vector<float> float_vec;

	float m_updateFrequencey;
	UINT m_nrPointsPerRow;
	UINT m_nrPointsPerCol;

	float_vec m_points;

public:

	Updater()
	{
	}

	virtual ~Updater()
	{
	}

	bool init(float updateFrequency, UINT pointsPerRow, UINT pointsPerCol, float base,
			  float interval);

	const float* const getPoints(){ return &m_points[0]; }
};
