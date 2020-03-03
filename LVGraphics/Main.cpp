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
	
	T::OBJMesh objmesh;
	bool loaded = objmesh.load("..\\ObjMesh\\soulspear.obj", false);
	//-------------------------------------------------------------------------------------------
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
	cam.setPerspective(FOV, aspectratio, 0.1f, 100.0f); //near and far
	cam.setLookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //from point up
	//cam.getWorldPos[1] = glm::vec4(0, 0, 0, 0);
	//cam.getview();
	//camera::setLookAt();,
	//view();

	//look at builds a view transform inverted of(0,0,1 that points in that direction from location 0,0,0 the Z axis for the camera is -Z the "up" direction is (0,1,0)   
	Shader shader("..\\Shaders\\normal_vertex.txt", "..\\Shaders\\fragment_light.txt"); //bind shader
	glm::mat4 model = glm::mat4(1.0f);
	//glm::mat4 models = glm::mat4(1.0f);
	//model[3] = glm::vec4(1, 0, 0, 1);

//loading shaders---------------------------------------------------------------------------------------
	//Shader shader("..\\Shaders\\simple_vertex.txt","..\\Shaders\\simple_color.txt" );

	struct Light
	{
		glm::vec3 direction0;
		glm::vec3 direction1;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	Light m_light;

	//depth testing disabled
	//alpha blend darw last
	//https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glBlendFunc.xml
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glEnable(GL_DEPTH_TEST);

	//textures-----------------------------------------------------------------------------
	uint m_texture;
	int x, y, n; // width, height ,channel

	unsigned char* Imagedata = stbi_load("..\\Images\\soulspear_diffuse.png", &x, &y, &n, 0);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, Imagedata);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	stbi_image_free(Imagedata);

	//Texture mytexture("..\\Images\\sadcat.jpg");
	//mytexture.getTexture();

	uint m_spectexture;
	//int X, Y, N;
	 unsigned char* dataimage =  stbi_load("..\\Images\\soulspear_specular.png", &x, &y, &n, 0);
	 glGenTextures(1, &m_spectexture);
	 glBindTexture(GL_TEXTURE_2D, m_spectexture);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	 stbi_image_free(dataimage);

//glBindTexture(GL_TEXTURE_2D, 0);
	 uint m_normaltexture;
	 //int X, Y, N;
	 dataimage = stbi_load("..\\Images\\soulspear_normal.png", &x, &y, &n, 0);
	 glGenTextures(1, &m_normaltexture);
	 glBindTexture(GL_TEXTURE_2D, m_normaltexture);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	 stbi_image_free(dataimage);
	 //stbi_set_flip_vertically_on_load(true);
	 //stbi_set_unpremultiply_on_load(true);
	//glBindTexture(GL_TEXTURE_2D, 0); //unbinding textures like pointing something to a nullptr
	
	uint m_cattexture;
	//int X, Y, N;
	dataimage = stbi_load("..\\Images\\sadcat.jpg", &x, &y, &n, 0);
	glGenTextures(1, &m_cattexture);
	glBindTexture(GL_TEXTURE_2D, m_cattexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(dataimage);


	uint m_Stexture;
	//int X, Y, N;
	dataimage = stbi_load("..\\Images\\specular.png", &x, &y, &n, 0);
	glGenTextures(1, &m_Stexture);
	glBindTexture(GL_TEXTURE_2D, m_Stexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataimage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(dataimage);

	uint m_bricktexture;
	//int X, Y, N;
	dataimage = stbi_load("..\\Images\\normal.jpg", &x, &y, &n, 0);
	glGenTextures(1, &m_bricktexture);
	glBindTexture(GL_TEXTURE_2D, m_bricktexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, dataimage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(dataimage);



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
		//models = glm::rotate(models, 0.0f, glm::vec3(0, 1, 0));
		//model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));

		//float deltatime = 0.0f;
		GLfloat currentTime = glfwGetTime();
		deltatime = currentTime - lastTime;
		lastTime = currentTime;
		cam.updatef(deltatime);
		

		
		//glUseProgram(shader_programID);
		//glUseProgram(shader.getshaderID());

		shader.BindShader();
		auto uniform_location = glGetUniformLocation(shader.getshaderID(), "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(cam.getprojectionviewtransform()));


		uniform_location = glGetUniformLocation(shader.getshaderID(), "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));

		//glm::mat3 ok = glm::mat3(glm::vec3(model[0]), glm::vec3(model[1]), glm::vec3(model[2])); ///why 
		uniform_location = glGetUniformLocation(shader.getshaderID(), "normal_matrix");
		//glUniformMatrix3fv(uniform_location, 1, false, glm::value_ptr(glm::inverseTranspose(ok)));
		glUniformMatrix3fv(uniform_location, 1, false, glm::value_ptr(glm::inverseTranspose(glm::mat3(model))));



		//-----------------------------------------------------------------
		auto pmv =	cam.getprojectionviewtransform() * model; //pmv
		


		//light roating 
		float timel = glfwGetTime();
	//	m_light.direction = glm::vec3(0, 0, 1);
		m_light.direction0 = glm::normalize(glm::vec3(glm::cos(timel * 2), glm::sin(timel * 2) , 0));
		//m_light.direction = glm::normalize(glm::vec3(glm::sin(timel / 2), 0, 0));
		m_light.direction1 = glm::normalize(glm::vec3(glm::cos(timel / 2), glm::sin(timel / 2), 0));

		//bind shader program 

		//bind light

		//bind transforms for lighting

		//draw

		uniform_location = glGetUniformLocation(shader.getshaderID(), "diffuseTexture");
		glUniform1i(uniform_location, 0);//D: 

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




		int Ia = glGetUniformLocation(shader.getshaderID(), "dirLights[0].Ia");
		glUniform3fv(Ia, 1.0f, glm::value_ptr(glm::vec3 (0.0, 0.0, 0.0))); //ambient lighting is purple
	

		int Id = glGetUniformLocation(shader.getshaderID(), "dirLights[0].Id"); //light diffuse 
		glUniform3fv(Id, 1.0f, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));

		int Is = glGetUniformLocation(shader.getshaderID(), "dirLights[0].Is"); //light specular color
		glUniform3fv(Is, 1.0f, glm::value_ptr(glm::vec3(0.25, 0.25, 0.25)));


		 Ia = glGetUniformLocation(shader.getshaderID(), "dirLights[1].Ia");
		//glUniform3fv(Ia, 1.0f, glm::value_ptr(glm::vec3 (0.30,0,0.70))); //ambient lighting is purple
		glUniform3fv(Ia, 1.0f, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

		 Id = glGetUniformLocation(shader.getshaderID(), "dirLights[1].Id"); //light diffuse 
		glUniform3fv(Id, 1.0f, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));

		 Is = glGetUniformLocation(shader.getshaderID(), "dirLights[1].Is"); //light specular colour
		glUniform3fv(Is, 1.0f, glm::value_ptr(glm::vec3(0.25, 0.25, 0.25)));


		int Ka = glGetUniformLocation(shader.getshaderID(), "Ka"); //material ambient
		glUniform3fv(Ka, 1.0f, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));


		int Kd = glGetUniformLocation(shader.getshaderID(), "Kd"); //material diffuse
		glUniform3fv(Kd, 1.0f, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));

		int Ks = glGetUniformLocation(shader.getshaderID(), "Ks"); // material specular colour
		glUniform3fv(Ks, 1.0f, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		//m_light.direction

		int LightDir = glGetUniformLocation(shader.getshaderID(), "dirLights[0].lightDirection");
		glUniform3fv(LightDir, 1.0f, glm::value_ptr((m_light.direction0)));//how the heck does this work 

		 LightDir = glGetUniformLocation(shader.getshaderID(), "dirLights[1].lightDirection");
		glUniform3fv(LightDir, 1.0f, glm::value_ptr((m_light.direction1)));//how the heck does this work 

		int camLocation = glGetUniformLocation(shader.getshaderID(), "cameraPosition");
		glUniform3fv(camLocation, 1.0f, glm::value_ptr(glm::vec3(cam.getworldtransform()[3])));


		//glm::vec3 N = glm::normalize(shader.getshaderID(), "vNormal");
	//glm::vec3 N = glm::normalize(glm::vec3(shader.getshaderID(), "vNormal",1));
		//glm::vec3 T = glm::normalize(glm::vec3(shader.getshaderID(), "vTangent",1));
		//glm::vec3 B = glm::normalize(glm::vec3(shader.getshaderID(), "vBiTangent", 1));
		

		/*glm::vec3 N = glm::normalize(glm::vec3(model, "vNormal", 1));

		glm::vec3 T = glm::normalize(glm::vec3(model, "vTangent", 1));
		glm::vec3 B = glm::normalize(glm::vec3(model, "vBiTangent", 1));


		glm::vec3 L = glm::normalize(glm::vec3(shader.getshaderID(), "lightDirection", 1));

		glm::mat4 TBN = glm::mat3(T, B, N);*/
		//specular power
		uniform_location = glGetUniformLocation(shader.getshaderID(), "SpecularPower");
		glUniform1f(uniform_location,20.f);
		//uniform_location = glGetUniformLocation(shader.getshaderID(), "time");
		//glUniform1f(uniform_location,currentTime);
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(currentTime) / 2.0f) + 0.5f;

		//glm::vec4 HSVcolor;
		//float hue = timeValue * 10.f; // f(sin)
		//HSVcolor = HSVtoColor(hue);
		//
		//int vertexcolorlocation = glGetUniformLocation(shader.getshaderID(), "color");
		//glUniform4fv(vertexcolorlocation, 1.0f, glm::value_ptr(HSVcolor));
		//glUniform4f(vertexcolorlocation, 1.0f, greenValue, 0.0f, 1.0f);
		//uniform_location = glGetUniformLocation(shader_programID, "color");
		//glUniformMatrix4fv(uniform_location, 1, glm::value_ptr(color)); 
		//glUniform4fv(uniform_location, 1, glm::value_ptr(color)); //aaaaaaaaaaaaaaaaaaaaa

	/*	float boarderColour[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, boarderColour);*/
		//stbi_set_flip_vertically_on_load(true);


		objmesh.draw();
		
	/*	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);*/
		//unbind shader
		//glUseProgram(0); // dont think thats right
		//bind the shader 
		//shader.BindShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_cattexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Stexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_bricktexture);

		
		meshloader.draw();

		
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
	