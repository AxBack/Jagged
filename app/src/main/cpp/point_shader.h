#pragma once

#include <string>
#include <android/asset_manager.h>
#include "pch.h"
#include "matrix.h"

class PointShader
{
private:

#define VIEW_PROJECTION "viewProjection"
#define XY "xy"
#define Z "z"

	GLuint m_vs;
	GLuint m_ps;
	GLuint m_program;

	GLint m_xyLocation;
	GLint m_zLocation;
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

	bool loadFile(AAssetManager* pAssetManager, const std::string& asset, std::string& outFile)
	{
		AAsset* pAsset = AAssetManager_open(pAssetManager, asset.c_str(), AASSET_MODE_STREAMING);
		if(pAsset == nullptr)
			return false;

		outFile.clear();

		static const size_t size = 1024;
		int r = 0;
		char* pBuffer = new char[size];
		while((r = AAsset_read(pAsset, pBuffer, size)) > 0)
		{
			outFile.insert(outFile.end(), pBuffer, pBuffer+r);
		}
		AAsset_close(pAsset);

		SAFE_DELETE_ARRAY(pBuffer);
		return true;
	}

public:

	PointShader()
			: m_program(0), m_vs(0), m_ps(0)
	{
	}

	bool init(AAssetManager* pAssetManager);

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

	GLuint getProgram() { return m_program; }
	GLuint getLocationXY() { return (GLuint)m_xyLocation; }
	GLuint getLocationZ() { return (GLuint)m_zLocation; }
	GLuint getViewProjectionLocation() {return m_viewProjectionLocation; }
};
