#pragma once

#include <string>
#include "pch.h"
#include "matrix.h"

class PointShader
{
private:

#define VIEW_PROJECTION "viewProjection"
#define POSITION "position"
#define COLOR "color"

	static const char* VS_CODE;
	static const char* PS_CODE;

	GLuint m_vs;
	GLuint m_ps;
	GLuint m_program;

	GLint m_positionLocation;
	GLint m_colorLocation;
	GLint m_viewProjectionLocation;

	bool verifyShader(GLuint shader)
	{
		GLint param = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
		if(param == GL_FALSE)
		{
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &param);
			GLchar* pLog = new GLchar[param];
			glGetShaderInfoLog(shader, param, nullptr, pLog);
			SAFE_DELETE_ARRAY(pLog);
		}
		return true;
	}

public:

	PointShader()
			: m_program(0), m_vs(0), m_ps(0)
	{
	}

	bool init()
	{
		m_vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_vs, 1, &VS_CODE, nullptr);
		glCompileShader(m_vs);
		if(!verifyShader(m_vs))
			return false;

		m_ps = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_ps, 1, &PS_CODE, nullptr);
		glCompileShader(m_ps);
		if(!verifyShader(m_ps))
			return false;

		m_program = glCreateProgram();
		glAttachShader(m_program, m_vs);
		glAttachShader(m_program, m_ps);
		glLinkProgram(m_program);

		GLint param = 0;
		glGetProgramiv(m_program, GL_LINK_STATUS, &param);
		if(param == GL_FALSE)
		{
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &param);
			GLchar* pLog = new GLchar[param];
			glGetProgramInfoLog(m_program, param, nullptr, pLog);
			SAFE_DELETE_ARRAY(pLog);
			return false;
		}

		m_positionLocation = glGetAttribLocation(m_program, POSITION);
		m_colorLocation = glGetAttribLocation(m_program, COLOR);
		m_viewProjectionLocation = glGetUniformLocation(m_program, VIEW_PROJECTION);

		return true;
	}

	void setViewProjection(Matrix viewProjection)
	{
		if(m_program == 0)
			return;

		if(m_viewProjectionLocation >= 0)
			glUniformMatrix4fv((GLuint)m_viewProjectionLocation, 1, GL_FALSE,
			                   viewProjection.data());
	}

	void release()
	{
		if(m_program > 0)
		{
			glDeleteProgram(m_program);
			m_program = 0;

			glDeleteShader(m_vs);
			glDeleteShader(m_ps);
		}
	}

	void bind()
	{
		GLsizei stride = sizeof(GLfloat) * 6;
		glUseProgram(m_program);
		glVertexAttribPointer((GLuint)m_positionLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray((GLuint)m_positionLocation);

		if(m_colorLocation >= 0)
		{
			GLsizei offset = sizeof(GLfloat) * 3;
			glVertexAttribPointer((GLuint)m_colorLocation, 3, GL_FLOAT, GL_FALSE, stride,
			                      (void*)&offset);
			glEnableVertexAttribArray((GLuint)m_colorLocation);
		}
	}
};
