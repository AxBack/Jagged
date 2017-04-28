
#include "point_shader.h"

bool PointShader::init(AAssetManager* pAssetManager)
{
	std::string file;
	if(!loadFile(pAssetManager, "shaders/PointShader.vs", file))
		return false;

	const char* pFile = file.c_str();

	m_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vs, 1, &pFile, nullptr);
	glCompileShader(m_vs);
	if(!verifyShader(m_vs))
		return false;

	if(!loadFile(pAssetManager, "shaders/PointShader.ps", file))
		return false;

	pFile = file.c_str();

	m_ps = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_ps, 1, &pFile, nullptr);
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

	m_xyLocation = glGetAttribLocation(m_program, XY);
	m_zLocation = glGetAttribLocation(m_program, Z);
	m_viewProjectionLocation = glGetUniformLocation(m_program, VIEW_PROJECTION);

	return true;
}