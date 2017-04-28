#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string.h>

#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = nullptr; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete [] ( p ); ( p ) = nullptr; } }

#define UINT unsigned int