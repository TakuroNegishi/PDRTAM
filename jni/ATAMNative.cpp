#include <jni.h>
#include <android/log.h>
//#include <hosei_negishi_pdrtam_DetectorNative.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "ATAM.h"
#include "GLRender.h"

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, __FILE__, __VA_ARGS__))

using namespace std;
using namespace cv;

extern "C" {
CATAM atam;
GLRender glRender;

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_initNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_mainProcNative(JNIEnv *env, jobject thiz, jlong addrRgba);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_changeStateNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_setStopNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_setResetNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_getPointAryNative(JNIEnv *env, jobject thiz, jint num, jfloatArray retObj);
JNIEXPORT jint JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_getPointLengthNative(JNIEnv *env, jobject thiz);

JNIEXPORT jint JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_getPointLengthNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_onSurfaceChangedNative(JNIEnv *env, jobject thiz, jint width, jint height);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_onDrawFrameNative(JNIEnv *env, jobject thiz);


JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_initNative(JNIEnv *env, jobject thiz) {
	atam.init();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_mainProcNative (JNIEnv *env, jobject thiz, jlong addrRgba) {
	Mat& mRgba = *(Mat*)addrRgba;
	atam.onceProc(mRgba);

	// TODO ここでATAM点群 >> GLRenderに移す

}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_changeStateNative(JNIEnv *env, jobject thiz) {
	atam.changeState();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_setStopNative(JNIEnv *env, jobject thiz) {
	atam.setStop();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_setResetNative(JNIEnv *env, jobject thiz) {
	atam.setReset();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_getPointAryNative(JNIEnv *env, jobject thiz, jint num, jfloatArray retObj) {
	int count = num;
	jfloat *ret = env->GetFloatArrayElements(retObj, NULL);

//	int size = atam.getPoint3Length();
	float pointAry[count*3];
	atam.getPoint3Float(pointAry);
	for (int i = 0; i < count*3; i++) {
		ret[i] = pointAry[i];
	}
	atam.setReset();

//	__android_log_print(ANDROID_LOG_ERROR, __FILE__, "count= %d \n", count);
//	__android_log_print(ANDROID_LOG_ERROR, __FILE__, "size= %d \n", size);

//	return pointAry;
	env->ReleaseFloatArrayElements(retObj, ret, 0);
}

JNIEXPORT jint JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_getPointLengthNative(JNIEnv *env, jobject thiz) {
	return atam.getPoint3Length();
}

// GLRender
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_onSurfaceCreatedNative(JNIEnv *env, jobject thiz) {
//	glRenderer = new GLRenderer();
	glRender.onSurfaceCreated();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_onSurfaceChangedNative(JNIEnv *env, jobject thiz, jint width, jint height) {
	glRender.onSurfaceChanged((int)width, (int)height);
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_app_NativeAccesser_onDrawFrameNative(JNIEnv *env, jobject thiz)  {
	glRender.onDrawFrame();
}

}
