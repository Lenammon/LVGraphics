#include "Mesh.h"
Mesh::Mesh()
{

}
Mesh::~Mesh()
{

}
void Mesh::initaliseQuad()
{
	assert(VAO == 0); //checks that the mesh is not initalized already 
	//generate buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO); //shoudlnt this go befor the vbo since the vao contains it
	//bind vertex array (mesh wrapper)
	glBindVertexArray(VAO); // is this what makes it contains the other two?
	//bind vertex buffer 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//define verts
	Vertex verties[6];
	verties[0].positon = { -0.5f, 0.0f, 0.5f, 1.0f }; //xyzt
	verties[1].positon = { 0.5f, 0.0f, 0.5f, 1.0f };
	verties[2].positon = { -0.5f, 0.0f, -0.5f, 1.0f };

	verties[3].positon = { -0.5f, 0.0f, -0.5f, 1.0f };
	verties[4].positon = { 0.5f, 0.0f, 0.5f, 1.0f };
	verties[5].positon = { 0.5f, 0.0f, -0.5f, 1.0f };
	//fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), verties, GL_STATIC_DRAW);
	//enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4 , GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//quad has 2 triangles
	triCount = 2;
}
void Mesh::draw()
{
	glBindVertexArray(VAO);
	if(IBO != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);

}