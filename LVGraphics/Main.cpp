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
#include "Texture.h"
#include <crtdbg.h>
using uint = unsigned int;
glm::vec4 HSVtoColor(float a_hue, float a_saturation = 1.0f, float a_value = 1.0f);

	GLfloat deltatime = 0.0f;
	GLfloat lastTime = 0.0f;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //checks for memory leaks
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


	

	

	//______________MESH______________________________________________________________________
	
	Mesh meshloader;
	meshloader.initaliseQuad();
	
	OBJMESH::OBJMesh objmesh;
	bool loaded = objmesh.load("..\\ObjMesh\\Human_Hand.obj", false);
	
	//---------------CAMERA----------------------------------------------------------------
	
	camera cam;
	
	
	float FOV = 1.570f;
	float aspectratio = 16 / 9.0f;
	cam.setPerspective(FOV, aspectratio, 0.1f, 100.0f); //near and far
	cam.setLookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //from point up
	

	//look at builds a view transform inverted of(0,0,1 that points in that direction from location 0,0,0 the Z axis for the camera is -Z the "up" direction is (0,1,0)   
	Shader shader("..\\Shaders\\normal_vertex.txt", "..\\Shaders\\fragment_light.txt"); //bind shader
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 model2 = glm::mat4(1.0f);
//	model[3] = glm::vec4(1, 0, 0, 1); // this moves the model


	struct Light
	{
		glm::vec3 direction0;
		glm::vec3 direction1;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	Light m_light;

	
	//enable depth testing 

	glEnable(GL_DEPTH_TEST);

	//textures-----------------------------------------------------------------------------
	uint m_texture;
	int x, y, n; // width, height ,channel

	unsigned char* Imagedata = stbi_load("..\\Images\\Hand_Albedo.png", &x, &y, &n, 0);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, Imagedata);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//mirros the texture
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	stbi_image_free(Imagedata);



	uint m_spectexture;
	//int X, Y, N;
	 unsigned char* dataimage =  stbi_load("..\\Images\\Hand_Spec.png", &x, &y, &n, 0);
	 glGenTextures(1, &m_spectexture);
	 glBindTexture(GL_TEXTURE_2D, m_spectexture);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataimage);

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	 stbi_image_free(dataimage);

//glBindTexture(GL_TEXTURE_2D, 0);
	 uint m_normaltexture;
	 //int X, Y, N;
	 dataimage = stbi_load("..\\Images\\Hand_Normal.png", &x, &y, &n, 0);
	 glGenTextures(1, &m_normaltexture);
	 glBindTexture(GL_TEXTURE_2D, m_normaltexture);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	 stbi_image_free(dataimage);
	 //stbi_set_flip_vertically_on_load(true);
	 //stbi_set_unpremultiply_on_load(true);
	//glBindTexture(GL_TEXTURE_2D, 0); //unbinding textures like pointing something to a nullptr

	//Diffuse Texture (Plane)
	uint m_Dtexture;
	dataimage = stbi_load("..\\Images\\Brickwall_Diffuse.jpg", &x, &y, &n, 0);
	glGenTextures(1, &m_Dtexture);
	glBindTexture(GL_TEXTURE_2D, m_Dtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(dataimage);

	//SpecularTexture (Plane)
	uint m_Stexture;
	dataimage = stbi_load("..\\Images\\Brickwall_Spec.jpg", &x, &y, &n, 0);
	glGenTextures(1, &m_Stexture);
	glBindTexture(GL_TEXTURE_2D, m_Stexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(dataimage);
	//NormalTexure (Plane)
	uint m_Ntexture;
	dataimage = stbi_load("..\\Images\\Brickwall_Normal.jpg", &x, &y, &n, 0);
	glGenTextures(1, &m_Ntexture);
	glBindTexture(GL_TEXTURE_2D, m_Ntexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(dataimage);



	//----------------------------------------------------------------------------------------------------------------------------
	glClearColor(0.5,0.5,0.5, 1.0); //make background grey
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
		

		
		GLfloat currentTime = glfwGetTime();
		deltatime = currentTime - lastTime;
		lastTime = currentTime;
		cam.updatef(deltatime);
		

		
		//binding shader------------------------------------

		shader.BindShader();
		auto uniform_location = glGetUniformLocation(shader.getshaderID(), "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(cam.getprojectionviewtransform()));


		uniform_location = glGetUniformLocation(shader.getshaderID(), "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));

		
		uniform_location = glGetUniformLocation(shader.getshaderID(), "normal_matrix");
		glUniformMatrix3fv(uniform_location, 1, false, glm::value_ptr(glm::inverseTranspose(glm::mat3(model))));



	
		auto pmv =	cam.getprojectionviewtransform() * model; //pmv
		


		//light roating 
		float timel = glfwGetTime();
		//first light
		m_light.direction0 = glm::normalize(glm::vec3(glm::cos(timel * 2), glm::sin(timel * 2) , 0));
		//second light
		m_light.direction1 = glm::normalize(glm::vec3(glm::cos(timel / 2), glm::sin(timel / 2), 0));

		
		
		uniform_location = glGetUniformLocation(shader.getshaderID(), "diffuseTexture");
		glUniform1i(uniform_location, 0);

		uniform_location = glGetUniformLocation(shader.getshaderID(), "specularTextureX");
		glUniform1i(uniform_location, 1);

		uniform_location = glGetUniformLocation(shader.getshaderID(), "normalTextureX"); //had the same name as the objmesh loader so it was clobering over it 
		glUniform1i(uniform_location, 2);

		///binding texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture); //diffuse texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_spectexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_normaltexture);


		//first Light

		int Ia = glGetUniformLocation(shader.getshaderID(), "dirLights[0].Ia");//light ambient 
		glUniform3fv(Ia, 1.0f, glm::value_ptr(glm::vec3 (0.0, 0.0, 0.0))); 
	

		int Id = glGetUniformLocation(shader.getshaderID(), "dirLights[0].Id"); //light diffuse 
		glUniform3fv(Id, 1.0f, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5))); //colour is gray

		int Is = glGetUniformLocation(shader.getshaderID(), "dirLights[0].Is"); //light specular color
		glUniform3fv(Is, 1.0f, glm::value_ptr(glm::vec3(0.25, 0.25, 0.25)));

		//second light 
		 Ia = glGetUniformLocation(shader.getshaderID(), "dirLights[1].Ia");//light ambient
		glUniform3fv(Ia, 1.0f, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

		 Id = glGetUniformLocation(shader.getshaderID(), "dirLights[1].Id"); //light diffuse 
		glUniform3fv(Id, 1.0f, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0))); //colour is blue

		 Is = glGetUniformLocation(shader.getshaderID(), "dirLights[1].Is"); //light specular colour
		glUniform3fv(Is, 1.0f, glm::value_ptr(glm::vec3(0.25, 0.25, 0.25)));

		//materials 
		int Ka = glGetUniformLocation(shader.getshaderID(), "Ka"); //material ambient
		glUniform3fv(Ka, 1.0f, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));


		int Kd = glGetUniformLocation(shader.getshaderID(), "Kd"); //material diffuse
		glUniform3fv(Kd, 1.0f, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));

		int Ks = glGetUniformLocation(shader.getshaderID(), "Ks"); // material specular colour
		glUniform3fv(Ks, 1.0f, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		
		//light direction 
		int LightDir = glGetUniformLocation(shader.getshaderID(), "dirLights[0].lightDirection");
		glUniform3fv(LightDir, 1.0f, glm::value_ptr((m_light.direction0)));

		 LightDir = glGetUniformLocation(shader.getshaderID(), "dirLights[1].lightDirection");
		glUniform3fv(LightDir, 1.0f, glm::value_ptr((m_light.direction1)));
		//camera position 
		int camLocation = glGetUniformLocation(shader.getshaderID(), "cameraPosition");
		glUniform3fv(camLocation, 1.0f, glm::value_ptr(glm::vec3(cam.getworldtransform()[3])));


		
		//specular power
		uniform_location = glGetUniformLocation(shader.getshaderID(), "SpecularPower");
		glUniform1f(uniform_location,20.0f);

	

		//draws the obj mesh (Hand)
		objmesh.draw();

		//binding the textures to the plane 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Dtexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Stexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_Ntexture);

		//draws the plane
		meshloader.draw();

		
		glfwSwapBuffers(window);
	}
	//glDeleteBuffers(1, &VAO);
	//	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
	
}
	
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
	