#pragma once
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "ext.hpp"
class camera
{
public:
	camera();

	~camera();
	virtual void update(float deltaTime); //why doesnt this need to match the cpp?

	glm::mat4 getview();
	glm::mat4 getworldtransform();
	glm::mat4 getprojectiontransform();
	glm::mat4 getprojectionviewtransform();
	glm::mat4 getMatrices();
	glm::vec3 getWorldPos(); //translation // position
	glm::vec3 getUpVec(); // check if correct
	void setPosition(glm::vec3 pos);
	void setPerspective(float FOV, float aspectRatio, float a_near, float a_far);
	void setLookAt(glm::vec3 from, glm::vec3 point, glm::vec3 up); //point = to
	void setLookAtPoint(glm::vec3 point);
	//free camera
	void updatef(float deltaTime);
	void setSpeed(float speed);

//private:
protected:
	glm::mat4 m_worldtransform;
	glm::mat4 m_viewtransform; // v
	glm::mat4 m_projectiontransform; //p
	glm::mat4 m_projectionviewtransform; // pv 
	////free camera
	float translation_speed = 5.0f;
	glm::vec3 m_up;
	float angularSpeed = 0.1f;
};
// add update matrices pmv 
//setting void (params)
//eg void setRotateX(float radians);
//getting param type ()
//eg float getRotationX();
