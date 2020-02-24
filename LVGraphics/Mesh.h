#pragma once
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "ext.hpp"
#include <assert.h>
#include <string>
#include <vector>
class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	void initaliseQuad();
	virtual void draw();

struct Vertex // strcuts are easier to copy for raw data
{
	glm::vec4 positon; //added to attribute location 0
	glm::vec2 UV; //textured co-ordination added to attribute location 2
	glm::vec4 normal; // added to attribute location 1
	//glm::vec4 color; //rgba
	glm::vec4 tangent; //added to attribute location 3

};

class material
{
	public:
	material(): ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1){}
	~material(){}
	glm::vec3 ambient;  // the colour of the object regardless of light
	glm::vec3 diffuse; //diffuse the colour of the object when illuminated, albedo is the colour of the object before its illuminated
	glm::vec3 specular; //the reflective colour of the surface
	glm::vec3 emissive; //the colour the object emits

	float specularPower;// controls the intesity or sharpness of the specular reflection
	float opacity;//how much of the object we can see 
	float Gloss; //shine


};
//bool load(const char* filename, bool loadtextures = true, bool flipTextureV = false);
//void draw(bool usePatches = false) override;
const std::string& getFilename() const { return m_filename; }

protected:
	unsigned int triCount;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
private:
	
	std::string m_filename;
	std::vector<material> m_materials;
};
