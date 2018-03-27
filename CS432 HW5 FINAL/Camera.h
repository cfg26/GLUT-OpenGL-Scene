#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"

class Camera {
public:
	Camera();
	mat4 getViewMatrix() const { return viewMatrix;}
	float getToggle() const { return toggle; }
	float getThetaMax() const { return toggle;}
	float getToggle2() const { return toggle2; }
	mat4 getProjectionMatrix() const { return projectionMatrix; }
	void positionCamera(vec4, vec4, vec4, vec4);
	void setProjection(mat4 p) { projectionMatrix = p; }
	void changeProjection(int width, int height);
	void setToggle(float temp);
	void setToggle2(float temp);
	void setThetaMax(float temp);
	void posYaw();
	void negYaw();
	void posPitch();
	void negPitch();
	void posRoll();
	void negRoll();
	
private:
	vec4 eye, u, v, n;
	mat4 viewMatrix, projectionMatrix;
	float toggle;
	float toggle2;
	float thetaMax;
};

#endif
