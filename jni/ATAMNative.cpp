#include <jni.h>
#include <android/log.h>
//#include <hosei_negishi_pdrtam_DetectorNative.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "ATAM.h"

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, __FILE__, __VA_ARGS__))

using namespace std;
using namespace cv;

extern "C" {
CATAM atam;

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_initNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_mainProcNative(JNIEnv *env, jobject thiz, jlong addrRgba);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_changeStateNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_setStopNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_setResetNative(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_getPointAryNative(JNIEnv *env, jobject thiz, jint num, jfloatArray retObj);
JNIEXPORT jint JNICALL Java_hosei_negishi_pdrtam_ATAMNative_getPointLengthNative(JNIEnv *env, jobject thiz);

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_initNative(JNIEnv *env, jobject thiz) {
	atam.init();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_mainProcNative (JNIEnv *env, jobject thiz, jlong addrRgba) {
	Mat& mRgba = *(Mat*)addrRgba;
	atam.onceProc(mRgba);

//	cv::cvtColor(mImg, mGimg, cv::COLOR_BGR2GRAY);

//	Mat& mRgba = *(Mat*)addrRgba;
//	Mat& mGray  = *(Mat*)addrGray;
//	vector<KeyPoint> v;
//
//	cv::Mat dstImg(640, 480, mGray.type());
//	cv::resize(mGray, dstImg, cv::Size(640, 480), 0, 0, cv::INTER_LINEAR);
//	float wRatio = mRgba.cols /640.0f;
//	float hRatio = mRgba.rows /480.0f;
//
//	cv::Ptr<cv::ORB> mDetector = cv::ORB::create(400, 1.2f, 2, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
//	mDetector->detect(dstImg, v);
//	// draw keypoint
//	for( unsigned int i = 0; i < v.size(); i++ ) {
//		const KeyPoint& kp = v[i];
//		circle(mRgba, Point(kp.pt.x * wRatio, kp.pt.y * hRatio), 10, Scalar(255, 0, 0, 255));
//	}
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_changeStateNative(JNIEnv *env, jobject thiz) {
	atam.changeState();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_setStopNative(JNIEnv *env, jobject thiz) {
	atam.setStop();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_setResetNative(JNIEnv *env, jobject thiz) {
	atam.setReset();
}

JNIEXPORT void JNICALL Java_hosei_negishi_pdrtam_ATAMNative_getPointAryNative(JNIEnv *env, jobject thiz, jint num, jfloatArray retObj) {
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

JNIEXPORT jint JNICALL Java_hosei_negishi_pdrtam_ATAMNative_getPointLengthNative(JNIEnv *env, jobject thiz) {
	return atam.getPoint3Length();
}

}
