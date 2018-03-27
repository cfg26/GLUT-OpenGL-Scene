#include "Camera.h"

Camera::Camera() {
	eye = vec4(0, 2, 0, 0);
	n = vec4(0, 0, 1, 0);
	v = vec4(0, 1, 0, 0);
	u = vec4(1, 0, 0, 0);
	viewMatrix = LookAt(eye, eye - n, v);
}

void Camera::changeProjection(int width, int height) {
	GLfloat fov = 65;
	GLfloat nearp = 0.2;
	GLfloat farp = 100.0;
	mat4 proj = Perspective(fov, GLfloat(width) / height, nearp, farp);
	setProjection(proj);
}

void Camera::positionCamera(vec4 ei, vec4 ni, vec4 vi, vec4 ui) {
	eye = ei;
	n = ni;
	v = vi;
	u = ui;
	viewMatrix = LookAt(eye, eye - n, v);
}

void Camera::setToggle(float temp) {
	toggle = temp;
}

void Camera::setToggle2(float temp)
{
	toggle2 = temp;
}

void Camera::setThetaMax(float temp)
{
	thetaMax = temp;
}


//yaw, roll, pitch commands
void Camera::posYaw() {
	v = v;
	n = cos(0.0698132)*n - sin(0.0698132)*u;
	u = sin(0.0698132)*n + cos(0.0698132)*u;
	viewMatrix = LookAt(eye, eye - n, v);

}

void Camera::negYaw()
{
	v = v;
	n = cos(-0.0698132)*n - sin(-0.0698132)*u;
	u = sin(-0.0698132)*n + cos(-0.0698132)*u;
	viewMatrix = LookAt(eye, eye - n, v);
}

void Camera::posPitch()
{
	u = u;
	v = cos(0.0698132)*v - sin(0.0698132)*n;
	n = sin(0.0698132)*v + cos(0.0698132)*n;
	viewMatrix = LookAt(eye, eye - n, v);
}

void Camera::negPitch()
{
	u = u;
	v = cos(-0.0698132)*v - sin(-0.0698132)*n;
	n = sin(-0.0698132)*v + cos(-0.0698132)*n;
	viewMatrix = LookAt(eye, eye - n, v);
}

void Camera::posRoll()
{
	n = n;
	v = cos(0.0698132)*v - sin(0.0698132)*u;
	u = sin(0.0698132)*v + cos(0.0698132)*u;
	viewMatrix = LookAt(eye, eye - n, v);
}

void Camera::negRoll()
{
	n = n;
	v = cos(-0.0698132)*v - sin(-0.0698132)*u;
	u = sin(-0.0698132)*v + cos(-0.0698132)*u;
	viewMatrix = LookAt(eye, eye - n, v);
}



