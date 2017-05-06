#pragma once

#include <android/asset_manager_jni.h>

#include <vector>
#include "matrix.h"
#include "pch.h"

#include "point_shader.h"
#include "updater.h"


class Engine
{
private:

#define SCALE_FACTOR 6
#define SIZE 60.0f
#define NR_BUFFERS 3
#define STATIC_BUFFER_INDEX 0
#define DYNAMIC_BUFFER_INDEX 1
#define INDEX_BUFFER_INDEX 2

	struct Point
	{
		GLfloat x,y;
	};

	Updater			m_updater;

	PointShader 	m_pointShader;

	GLuint 			m_nrPointsPerX;
	GLuint 			m_nrPointsPerY;

	GLuint			m_nrIndices;

	Matrix          m_viewProjection;

	GLuint 			m_buffers[NR_BUFFERS];

public:

	Engine()
	: m_nrPointsPerX(0)
	, m_nrPointsPerY(0)
	{
		memset(m_buffers, 0, NR_BUFFERS);
		Matrix::identity(m_viewProjection);
	}

	virtual ~Engine()
	{
		clear();
		m_pointShader.release();
	}

	void clear();

	bool init(AAssetManager* pAssetManager, int width, int height);

	bool render();

	void resume() { m_updater.resume(); }
	void pause() { m_updater.pause(); }

	void touch(float x, float y);
};

