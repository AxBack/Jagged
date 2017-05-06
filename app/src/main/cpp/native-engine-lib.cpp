#include <jni.h>

#include "engine.h"

Engine* m_pEngine = nullptr;

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_wallpaper_axb_jagged_NativeEngine_create(JNIEnv* env, jobject /* this */,
												  jobject assetManager,
												  int width, int height)
{
	if(m_pEngine != nullptr)
		SAFE_DELETE(m_pEngine);

	m_pEngine = new Engine;
	AAssetManager* pAssetManager = AAssetManager_fromJava(env, assetManager);
	return m_pEngine->init(pAssetManager, width, height);
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_jagged_NativeEngine_destroy(JNIEnv* pEnv, jobject /*this */)
{
	SAFE_DELETE(m_pEngine);
}

JNIEXPORT jboolean JNICALL
Java_com_wallpaper_axb_jagged_NativeEngine_render(JNIEnv* pEnv, jobject /*this */)
{
	if(m_pEngine)
		return m_pEngine->render();

	return JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_jagged_NativeEngine_resume(JNIEnv* pEnv, jobject /*this*/)
{
	if(m_pEngine)
		m_pEngine->resume();
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_jagged_NativeEngine_pause(JNIEnv* pEnv, jobject /*this*/)
{
	if(m_pEngine)
		m_pEngine->pause();
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_jagged_NativeEngine_onTouch(JNIEnv* pEnv, jobject /*this */, float x,
												   float y)
{
	if(m_pEngine)
		m_pEngine->touch(x,y);
}

}