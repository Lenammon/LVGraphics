#include "glm.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "ext.hpp"
#include <fstream>
#include <sstream>
#include"camera.h"
#include <iostream>
#include "Mesh.h"
#include "OBJMesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"

using uint = unsigned int;
glm::vec4 HSVtoColor(float a_hue, float a_saturation = 1.0f, float a_value = 1.0f);

	GLfloat deltatime = 0.0f;
	GLfloat lastTime = 0.0f;

int main()
{
	if (glfwInit() == false) 
	{
		return -1; 
	}
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello World", nullptr, nullptr);
	if(window == nullptr)
	{
		glfwTerminate();
		return-2;
	}
	glfwMakeContextCurrent(window);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	//cant get open gl until we call this^

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	//mesh data
	//glm::vec3 verticies[] =
	//{
	//	//back
	//	glm::vec3(-0.5f, 0.5f, 0.0f),
	//	glm::vec3(-0.5f, -0.5f, 0.0f),
	////	glm::vec3(0.5f, 0.5f, 0.0f),
	//	glm::vec3(0.5f, 0.5f, 0.0f),
	////	glm::vec3(-0.5f, -0.5f, 0.0f),
	//	glm::vec3(0.5f, -0.5f, 0.0f)
	////	//front
	////	/*glm::vec3(-0.5f, 0.5f, -1.0f),
	////	glm::vec3(0.5f, 0.5f, -1.0f),
	////	glm::vec3(-0.5f, -0.5f, -1.0f),
	////	glm::vec3(0.5f, 0.5f, -1.0f),
	////	glm::vec3(-0.5f, -0.5f, -1.0f),
	////	glm::vec3(0.5f, -0.5f, -1.0f)*/
	////	//this is not a square
	////	//front
	////	/*glm::vec3(-1.0f, -1.0f, 1.0f),
	////	glm::vec3(1.0f, -1.0f, 1.0f),
	////	glm::vec3(1.0f, 1.0f, 1.0f),
	////	glm::vec3(-1.0f, 1.0f, 1.0f),
	////	//back
	////	glm::vec3(-1.0f, -1.0f, -1.0f),
	////	glm::vec3(1.0f, -1.0f, -1.0f),
	////	glm::vec3(1.0f, 1.0f, -1.0f),
	////	glm::vec3(-1.0f, 1.0f, -1.0f),*/

	//};
	//int number_of_vert = 12;

	//int  index_buffer[]{ 0,1,2,1,3,2 }; // abcbdc

	////create and load mesh review this 
	//uint VAO;
	//glGenVertexArrays(1, &VAO);
	//uint VBO;
	//glGenBuffers(1, &VBO);
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	////for vbo or vao?
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), verticies, GL_STATIC_DRAW);//glBufferData(GL_ARRAY_BUFFER, number of verts * sizeof(vertStruct), verticies, GL_STATIC_DRAW);
	//// index buffer object
	//uint IBO;
	//glGenBuffers(1, &IBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), index_buffer, GL_STATIC_DRAW);

	////avo vbo
	//glEnableVertexAttribArray(0); //enables the generic vertex attribute array specified by index
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	
	Mesh meshloader;
	meshloader.initaliseQuad();
	
//	T::OBJMesh objmesh;
	//bool loaded = objmesh.load("..\\ObjMesh\\Bunny.obj", false);
	
	//objmesh.draw();
	//meshloader.draw();
	//PMV---------------CAMERA----------------------------------------------------------------
	//glm::mat4 projection = glm::perspective(1.570f, 16 / 9.0f, 0.1f, 50.0f); //glm ortho camera
	
	
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0,0,0), glm::vec3(0, 1, 0)); // eye
	camera cam;
	//cam::setLookAt(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //brings up errors
	//camera::getview();
	
	float FOV = 1.570f;
	float aspectratio = 16 / 9.0f;
	cam.setPerspective(FOV, aspectratio, 0.1f, 100.0f);
	cam.setLookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//cam.getview();
	//camera::setLookAt();,
	//view();
	//look at builds a view transform inverted of(0,0,1 that points in that direction from location 0,0,0 the Z axis for the camera is -Z the "up" direction is (0,1,0)   
	glm::mat4 model = glm::mat4(1.0f);
	
//loading shaders---------------------------------------------------------------------------------------
	//Shader shader("..\\Shaders\\simple_vertex.txt","..\\Shaders\\simple_color.txt" );
	Shader shader("..\\Shaders\\normal_vertex.txt", "..\\Shaders\\fragment_light.txt");

	struct Light
	{
		glm::vec3 direction;
	};

	Light m_light;




	glEnable(GL_DEPTH_TEST);

	//textures-----
	uint m_texture;
	int x, y, n; // width, height ,channel

	unsigned char* Imagedata = stbi_load("..\\Images\\swirl.png", &x, &y, &n, 0);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Imagedata);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	stbi_image_free(Imagedata);



	glBindTexture(GL_TEXTURE_2D, 0); //unbinding textures like pointing something to a nullptr

	//------------------------------------------------------------------------------
	glClearColor(0.5,0.5,0.5, 1.0); //make background white 
	glPolygonMode(GL_BACK, GL_LINE);
	while(glfwWindowShouldClose(window) == false && glfwGetKey(window,GLFW_KEY_ESCAPE)!= GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		
		//glm::mat4 pvm = projection * view * model;
		//glm::mat4 pv = projection * view;
		//glm::vec4 color = glm::vec4(0.8f, 0.0f, 0.2f, 1.0f); //RGBA
		//rotation 

		model = glm::rotate(model, 0.0f, glm::vec3(0, 1,0));//when no roation is applied the bunny cannot be found 
		//thats because it rotating around any axis and it collaspses in on itself needs to be exaclty 1 or 0 ^

		//model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));

		//float deltatime = 0.0f;
		GLfloat currentTime = glfwGetTime();
		deltatime = currentTime - lastTime;
		lastTime = currentTime;
		cam.updatef(deltatime);
		

		
		//glUseProgram(shader_programID);
		//glUseProgram(shader.getshdaerID());

		shader.BindShader();
		auto uniform_location = glGetUniformLocation(shader.getshdaerID(), "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(cam.getprojectionviewtransform()));


		uniform_location = glGetUniformLocation(shader.getshdaerID(), "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));

		uniform_location = glGetUniformLocation(shader.getshdaerID(), "normal_matrix");
		glUniformMatrix3fv(uniform_location, 1, false, glm::value_ptr(glm::inverseTranspose(glm::mat3(model))));


		//-----------------------------------------------------------------
		auto pmv =	cam.getprojectionviewtransform() * model; //pmv
		


		//light roating 
		float timel = glfwGetTime();
		m_light.direction = glm::normalize(glm::vec3(glm::cos(timel * 2), glm::sin(timel * 2) , 0));
		//m_light.direction = glm::normalize(glm::vec3(glm::sin(timel / 2), 0, 0));
		//bind shader program 

		//bind light

		//bind transforms for lighting

		//draw

		glBindTexture(GL_TEXTURE_2D, m_texture); //diffuse texture
		

		int Ia = glGetUniformLocation(shader.getshdaerID(), "Ia");
		glUniform3fv(Ia, 1.0f, glm::value_ptr(glm::vec3 (0.25,0.25,0.0))); //ambient lighting


		int Id = glGetUniformLocation(shader.getshdaerID(), "Id"); //light diffues 
		glUniform3fv(Id, 1.0f, glm::value_ptr(glm::vec3(0.75, 0.75, 0.75)));

		int Is = glGetUniformLocation(shader.getshdaerID(), "Is"); //light specular
		glUniform3fv(Is, 1.0f, glm::value_ptr(glm::vec3(0.80, 0.80, 0.80)));


		int Ka = glGetUniformLocation(shader.getshdaerID(), "Ka"); //material ambient
		glUniform3fv(Ka, 1.0f, glm::value_ptr(glm::vec3(0.25, 0.25, 0.25)));


		int Kd = glGetUniformLocation(shader.getshdaerID(), "Kd"); //material diffuse
		glUniform3fv(Kd, 1.0f, glm::value_ptr(glm::vec3(0.75, 0.75, 0.75)));

		int Ks = glGetUniformLocation(shader.getshdaerID(), "Ks"); // material specular
		glUniform3fv(Ks, 1.0f, glm::value_ptr(glm::vec3(0.25, 0.25, 0.25)));
		//m_light.direction

		int LightDir = glGetUniformLocation(shader.getshdaerID(), "lightDirection");
		glUniform3fv(LightDir, 1.0f, glm::value_ptr((m_light.direction)));

		int camLocation = glGetUniformLocation(shader.getshdaerID(), "cameraPosition");
		glUniform3fv(camLocation, 1.0f, glm::value_ptr(glm::vec3(cam.getworldtransform()[3])));

		//specular power
		uniform_location = glGetUniformLocation(shader.getshdaerID(), "SpecularPower");
		glUniform1f(uniform_location,20.f);
		//uniform_location = glGetUniformLocation(shader.getshdaerID(), "time");
		//glUniform1f(uniform_location,currentTime);
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(currentTime) / 2.0f) + 0.5f;

		//glm::vec4 HSVcolor;
		//float hue = timeValue * 10.f; // f(sin)
		//HSVcolor = HSVtoColor(hue);
		//
		//int vertexcolorlocation = glGetUniformLocation(shader.getshdaerID(), "color");
		//glUniform4fv(vertexcolorlocation, 1.0f, glm::value_ptr(HSVcolor));
		//glUniform4f(vertexcolorlocation, 1.0f, greenValue, 0.0f, 1.0f);
		//uniform_location = glGetUniformLocation(shader_programID, "color");
		//glUniformMatrix4fv(uniform_location, 1, glm::value_ptr(color)); 
		//glUniform4fv(uniform_location, 1, glm::value_ptr(color)); //aaaaaaaaaaaaaaaaaaaaa


		meshloader.draw();
		//objmesh.draw();


		//glBindVertexArray(VAO);
	//	glDrawArrays(GL_TRIANGLES, 0, number_of_vert);s
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}
	//glDeleteBuffers(1, &VAO);
//	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
	
}
	//void cam()
	//{
	//	//glm::mat4 projection = glm::perspective(1.570f, 16 / 9.0f, 0.1f, 50.0f); //glm ortho camera
	//}
	//https://bitbucket.org/Milknife/raylib-nystrom-dungeon/src/master/RaylibStarterCS/Project2D/NystromRenderer.cs 
	//https://au.mathworks.com/help/images/convert-from-hsv-to-rgb-color-space.html

	glm::vec4 HSVtoColor(float a_hue, float a_saturation, float a_value )
	{
		float primaryChroma = a_value * a_saturation;
		float secondaryChroma = primaryChroma * (1.0f - abs((a_hue / 60.0f) - (2 *((int)(a_hue / 60.0f) / 2)) - 1.0f)); //the mod of a floating point div is the difference of the numerator and its int div 
		//float secondaryChroma = primaryChroma * (1.0f - abs((a_hue / 60.0f) % 2) - 1.0f)));
		//float secondaryChroma = primaryChroma * (1.0f - abs(fmod((a_hue / 60.0f), 2.0f)) - 1.0f);
		float offset = a_value - primaryChroma;

		float rPrime = 0;
		float gPrime = 0;
		float bPrime = 0;

		// Crawl around the six triangles on the hue plane
		// to Find primary and secondary chroma
		if (a_hue >= 0 && a_hue < 60)
		{
			rPrime = primaryChroma;
			gPrime = secondaryChroma;
			bPrime = 0;
		}
		else if (a_hue >= 60 && a_hue < 120)
		{
			rPrime = secondaryChroma;
			gPrime = primaryChroma;
			bPrime = 0;
		}
		else if (a_hue >= 120 && a_hue < 180)
		{
			rPrime = 0;
			gPrime = primaryChroma;
			bPrime = secondaryChroma;
		}
		else if (a_hue >= 180 && a_hue < 240)
		{
			rPrime = 0;
			gPrime = secondaryChroma;
			bPrime = primaryChroma;
		}
		else if (a_hue >= 240 && a_hue < 300)
		{
			rPrime = secondaryChroma;
			gPrime = 0;
			bPrime = primaryChroma;
		}
		else// if (a_hue >= 0 && a_hue < 360)
		{
			rPrime = primaryChroma;
			gPrime = 0;
			bPrime = secondaryChroma;
		}

		// Build the colour from 0.0-1.0f into 0-255 bytes
		
		//std::cout << a_hue << std::endl; 
		return glm::vec4((rPrime + offset), (gPrime + offset) , (bPrime + offset) , 1.0f);
	}
	