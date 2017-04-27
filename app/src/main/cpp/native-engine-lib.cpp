#include <jni.h>

#include "engine.h"

Engine* m_pEngine = nullptr;

extern "C" {

JNIEXPORT jboolean JNICALL Java_com_wallpaper_axb_jagged_NativeEngine_create(JNIEnv* env, jobject /* this */,
                                                                             int width, int height)
{
	m_pEngine = new Engine;
	return m_pEngine->init(width, height);
}

JNIEXPORT void JNICALL Java_com_wallpaper_axb_jagged_NativeEngine_destroy(JNIEnv* pEnv, jobject /*this */)
{
	SAFE_DELETE(m_pEngine);
}

JNIEXPORT jboolean JNICALL Java_com_wallpaper_axb_jagged_NativeEngine_render(JNIEnv* pEnv, jobject /*this */)
{
	if(m_pEngine)
		return m_pEngine->render();

	return JNI_FALSE;
}

}