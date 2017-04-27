#include "engine.h"

void Engine::clear()
{
	m_points.clear();
	m_indices.clear();
	if(m_buffers[0] == 0)
	{
		glDeleteBuffers(NR_BUFFERS, m_buffers);
		m_buffers[0] = m_buffers[1] = 0;
	}
}

bool Engine::init(int width, int height)
{
	clear();

	if(!m_pointShader.init())
		return false;

	static const float size = 100.0f;

	Matrix projection, view;

	float halfX = static_cast<float>(width) * 0.5f;
	float halfY = static_cast<float>(width) * 0.5f;

	Matrix::frustum(projection, -halfX, halfX, -halfY, halfY, 0.1, 100);
	Matrix::lookAt(view, {halfX, halfY, -0.1f}, {halfX, halfY, 1}, {0, 1, 0});
	m_viewProjection = projection * view;

	m_nrPointsPerX = static_cast<GLuint>(static_cast<GLfloat>(width) / size) + 1;
	m_nrPointsPerY = static_cast<GLuint>(static_cast<GLfloat>(height) / size) + 1;

	unsigned int nrPoints = m_nrPointsPerX * m_nrPointsPerY;
	m_points.resize(nrPoints);

	float offsetX = static_cast<float>(width) / static_cast<GLfloat>(m_nrPointsPerX-1);
	float offsetY = static_cast<float>(height) / static_cast<GLfloat>(m_nrPointsPerY-1);

	for(unsigned int y = 0; y < m_nrPointsPerY; ++y)
	{
		for(unsigned int x = 0; x < m_nrPointsPerX; ++x)
		{
			unsigned int index = y * m_nrPointsPerX + x;
			m_points[index] = {
					x * offsetX,
					y * offsetY,
					0,
					static_cast<GLfloat>(x) / static_cast<GLfloat>(m_nrPointsPerX),
					static_cast<GLfloat>(y) / static_cast<GLfloat>(m_nrPointsPerY),
					static_cast<GLfloat>(x + y) /
						static_cast<GLfloat>(m_nrPointsPerX + m_nrPointsPerY),
			        1
			};

			if(y < m_nrPointsPerY-1 && x < m_nrPointsPerX)
			{
				if(x != 0)
				{
					m_indices.insert(m_indices.end(), {
							static_cast<GLushort>(index + m_nrPointsPerX - 1),
							static_cast<GLushort>(index),
							static_cast<GLushort>(index + m_nrPointsPerX)
					});
				}

				if( x < m_nrPointsPerX - 1)
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[IBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * m_indices.size(), &m_indices[0],
	             GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * m_points.size(), &m_points[0],
	             GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_NONE);

	return true;
}

bool Engine::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBO]);

	m_pointShader.bind();
	m_pointShader.setViewProjection(m_viewProjection);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[IBO]);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}