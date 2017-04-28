#include "engine.h"

void Engine::clear()
{
	m_points.clear();
	m_indices.clear();
	if(m_buffers[0] == 0)
	{
		glDeleteBuffers(NR_BUFFERS, m_buffers);
		for(unsigned int i = 0; i < NR_BUFFERS; ++i)
			m_buffers[i] = 0;
	}
}

bool Engine::init(AAssetManager* pAssetManager, int width, int height)
{
	clear();

	if(!m_pointShader.init(pAssetManager))
		return false;

	static const float size = 60.0f;

	Matrix projection, view;

	m_nrPointsPerX = static_cast<GLuint>(width / size) + 1;
	m_nrPointsPerY = static_cast<GLuint>(height / size) + 1;

	float halfX = static_cast<float>(width) * 0.5f;
	float halfY = static_cast<float>(height) * 0.5f;

	float w = width * SCALE_FACTOR;
	float h = height * SCALE_FACTOR;

	float x = w * 0.5f;
	float y = h * 0.5f;

	Matrix::frustum(projection, -halfX, halfX, -halfY, halfY, 10, 110);
	Matrix::lookAt(view, {x, y, -10.0f}, {x, y, 1}, {0, 1, 0});
	m_viewProjection = projection * view;

	UINT nrPoints = m_nrPointsPerX * m_nrPointsPerY;
	m_points.resize(nrPoints);

	float offsetX = (w / static_cast<GLfloat>(m_nrPointsPerX - 1));
	float offsetY = (h / static_cast<GLfloat>(m_nrPointsPerY - 1));

	for(unsigned int y = 0; y < m_nrPointsPerY; ++y)
	{
		for(unsigned int x = 0; x < m_nrPointsPerX; ++x)
		{
			unsigned int index = y * m_nrPointsPerX + x;
			m_points[index] = {
					x * offsetX,
					y * offsetY
			};

			if(y < m_nrPointsPerY - 1 && x < m_nrPointsPerX)
			{
				if(x != 0)
				{
					m_indices.insert(m_indices.end(), {
							static_cast<GLushort>(index + m_nrPointsPerX - 1),
							static_cast<GLushort>(index),
							static_cast<GLushort>(index + m_nrPointsPerX)
					});
				}

				if(x < m_nrPointsPerX - 1)
				{
					m_indices.insert(m_indices.end(), {
							static_cast<GLushort>(index),
							static_cast<GLushort>(index + 1),
							static_cast<GLushort>(index + m_nrPointsPerX)
					});
				}
			}
		}
	}

	glGenBuffers(NR_BUFFERS, m_buffers);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indices.size(), &m_indices[0],
				 GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[STATIC_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * m_points.size(), &m_points[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[DYNAMIC_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_points.size(), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBlendColor(0, 0, 0, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	if(!m_updater.init(1.0f/60.0f, m_nrPointsPerX, m_nrPointsPerY, 50, size))
		return false;

	return true;
}

bool Engine::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_pointShader.getProgram());
	glUniformMatrix4fv(m_pointShader.getViewProjectionLocation(), 1, GL_FALSE,
					   m_viewProjection.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[DYNAMIC_BUFFER_INDEX]);
	Point* p = (Point*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float) * m_points.size(),
										GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	memcpy(p, m_updater.getPoints(), sizeof(float) * m_points.size());
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glVertexAttribPointer(m_pointShader.getLocationZ(), 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
	glEnableVertexAttribArray(m_pointShader.getLocationZ());

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[STATIC_BUFFER_INDEX]);
	glVertexAttribPointer(m_pointShader.getLocationXY(), 2, GL_FLOAT, GL_FALSE, sizeof(Point), 0);
	glEnableVertexAttribArray(m_pointShader.getLocationXY());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER_INDEX]);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}
