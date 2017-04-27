
#include "point_shader.h"

const char* PointShader::VS_CODE = "#version 300 es \n"
"uniform mat4 viewProjection;"

"in vec3 position;"
"in vec4 color;"
""
"out vec4 vertexColor;"
"void main() {"
"vertexColor = color;"
"gl_Position = viewProjection * vec4(position, 1);"
"} \0";

const char* PointShader::PS_CODE = "#version 300 es \n"
"precision mediump float;"
"in vec4 vertexColor;"
"out vec4 finalColor;"
"void main() {"
"finalColor = vertexColor;"
"} \0";