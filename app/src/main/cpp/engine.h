#pragma once

#include <vector>
#include "matrix.h"
#include "pch.h"

#include "point_shader.h"

class Engine
{
private:

	struct Point
	{
		GLfloat x,y,z, r,g,b,a;
	};

	PointShader m_pointShader;

	GLuint m_nrPointsPerX;
	GLuint m_nrPointsPerY;

	Matrix              m_viewProjection;
	std::vector<Point>  m_points;
	std::vector<GLushort> m_indices;

#define NR_BUFFERS 2
#define VBO 0
#define IBO 1

	GLuint m_buffers[NR_BUFFERS];

public:

	Engine()
	: m_nrPointsPerX(0)
	, m_nrPointsPerY(0)
	{
		memset(m_buffers, 0, NR_BUFFERS);
	}

	virtual ~Engine()
	{
		clear();
		m_pointShader.release();
	}

	void clear();

	bool init(int width, int height);

	bool render();

};

