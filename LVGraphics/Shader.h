#pragma once
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "ext.hpp"
#include <fstream>
#include <sstream>
class Shader
{
public:
	//should i have another constructor here to set all the veriables


	Shader(std::string vertex, std::string frag);
	~Shader();
	//void LoadShader();
//	void Buildshader(); 
	void ErrorCheckVertexShader(); 
	void ErrorCheckFragShader();
	void ErrorCheckLinker();
	//void Allocatespace();
	//get complied shader? i dont knwo what i am doing D:
	unsigned int getshdaerID()
	{
		return shader_program_ID;
	}
	void BindShader();
private:

	unsigned int vertex_shader_ID;
	unsigned int fragment_shader_ID;
	unsigned int shader_program_ID;

	std::string shader_data;
	std::stringstream string_stream;

	const char* data;
	GLint success;

};

