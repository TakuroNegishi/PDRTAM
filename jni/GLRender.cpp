/*!
@file		GLRender.cpp
@brief		functions in GLRender
*/

#include "GLRender.h"
//#include <jni.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <android/log.h>
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, __FILE__, __VA_ARGS__))

/*!
@brief		constructor
*/
GLRender::GLRender()
{

}

/*!
@brief		destructor
*/
GLRender::~GLRender()
{
}

/*!
@brief		initialize parameters
*/
void GLRender::onSurfaceCreated()
{
	eyeX = 30.0f;
	eyeY = 30.0f;
	eyeZ = 30.0f;
	centerX = 0.0f;
	centerY = 0.0f;
	centerZ = 0.0f;
	rotateX = 0.0f;
	rotateY = 0.0f;
	rotateZ = 0.0f;
}

void GLRender::onSurfaceChanged(int width, int height)
{
	// 左下が(0,0)
	// width, height(画面いっぱいまで表示に使う)
	screenW = width;
	screenH = height;
	glViewport(0, 0, width, height);
	aspect = (float) width / (float) height;
	glEnable(GL_DEPTH_TEST);	// 深度バッファを有効にする

}

void GLRender::onDrawFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// カメラ
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	gluPerspective(45.0f, aspect, 0.01f, 200.0f);
	// カメラの位置, 注視位置, 上方向ベクトル
//	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);

	// モデル
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// 回転
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	glRotatef(rotateZ, 0, 0, 1);


	// 線を引く
	float one = 25.0f;
	float v[] = { -one, 0.0f, 0.0f, one, 0.0f, 0.0f };
	glLineWidth(5);
	glEnableClientState(GL_VERTEX_ARRAY);	// 頂点座標を有効
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, v);	// 3=(x, y, z) 頂点座標登録
	glDrawArrays(GL_LINES, 0, 2); // 描画
	v[0] = 0.0f;
	v[3] = 0.0f;
	v[1] = -one;
	v[4] = one;
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertexPointer(3, GL_FLOAT, 0, v);	// 3=(x, y, z) 頂点座標登録
	glDrawArrays(GL_LINES, 0, 2); // 描画
	v[1] = 0.0f;
	v[4] = 0.0f;
	v[2] = -one;
	v[5] = one;
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, v);	// 3=(x, y, z) 頂点座標登録
	glDrawArrays(GL_LINES, 0, 2); // 描画

//	glDisable(GL_TEXTURE_2D); // テクスチャ無効
}

void GLRender::onDestroy()
{

}

