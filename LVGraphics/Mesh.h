#pragma once
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "ext.hpp"
#include <assert.h>
class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	void initaliseQuad();
	virtual void draw();

protected:
	unsigned int triCount;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
};
struct Vertex // strcuts are easier to copy for raw data
{
	glm::vec4 positon;
	glm::vec4 normal;
	//glm::vec4 color; //rgba
	glm::vec2 UV; //textured co-ordination

};
