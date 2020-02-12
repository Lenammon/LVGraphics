#include "Mesh.h"
Mesh::Mesh()
{

}
Mesh::~Mesh()
{

}
void Mesh::initaliseQuad()
{
	//assert(VAO == 0); //checks that the mesh is not initalized already 
	//generate buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO); //shoudlnt this go befor the vbo since the vao contains it
	//bind vertex array (mesh wrapper)
	glBindVertexArray(VAO); // is this what makes it contains the other two?
	//bind vertex buffer 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//this creats a unit quad the sides have the length of one
	//define verts
	Vertex verties[6];
	verties[0].positon = { -0.5f, 0.0f, 0.5f, 1.0f }; //xyzt
	verties[1].positon = { 0.5f, 0.0f, 0.5f, 1.0f };
	verties[2].positon = { -0.5f, 0.0f, -0.5f, 1.0f };

	verties[4].positon = { 0.5f, 0.0f, 0.5f, 1.0f };
	verties[3].positon = { 0.5f, 0.0f, -0.5f, 1.0f };
	verties[5].positon = { -0.5f, 0.0f, -0.5f, 1.0f };


	int index_buffer[]{ 0,1,2,1,3,2 };
	triCount = 6;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triCount * sizeof(int), &index_buffer[0], GL_STATIC_DRAW);
	//fill vertex buffer
	//glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), &verties[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), &verties[0], GL_STATIC_DRAW); // needs to be fixed-- if the other vectors are in the struct it doesnt work  
	//enable first element as position
	glEnableVertexAttribArray(0); //enables the generic vertex attribute array specified by index
	glVertexAttribPointer(0, 4 , GL_FLOAT, GL_FALSE,   sizeof(Vertex), 0);
	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*auto window = glfwGetCurrentContext();
	if(glfwGetKey(window,GLFW_KEY_X))
	{
		for(int i = 0; i < 0 ; i++)
		{
			verties[i].positon;
		}
	}*/

	//quad has 2 triangles
}
void Mesh::draw()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//checks to see if your using indices or just verticies
	if(IBO != 0) // indices
	{
		glDrawElements(GL_TRIANGLES, triCount, GL_UNSIGNED_INT, 0);
	}
	else // verticies
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//void Mesh::draw(bool usePatches)
//{
//
//}
//bool Mesh::load(const char*, bool loadTextures, bool flipTextureV )
//{
//	return false;
//}
