/*!
@file		GLRender.h
@brief		header of GLRender
*/
#pragma once

/*!
@class		GLRender
@brief
*/
class GLRender
{
private:
	float aspect;
	int screenW;
	int screenH;
	float eyeX;
	float eyeY;
	float eyeZ;
	float centerX;
	float centerY;
	float centerZ;
	float rotateX;
	float rotateY;
	float rotateZ;

public:
	GLRender();
	~GLRender();

public:
	void onSurfaceCreated();
	void onSurfaceChanged(int width, int height);
	void onDrawFrame();
	void onDestroy();
};

