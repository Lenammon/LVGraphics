//#include "Texture.h"
//Texture::Texture( const char* texture/*std::string texture, std::string specular, std::string normal*/)
//{
//	int x, y, n; // width, height ,channel
//
//	unsigned char* Imagedata = stbi_load(texture, &x, &y, &n, 0);
//	//unsigned char* Imagedata = stbi_load(texture, &x, &y, &n, 0);
//	
//	glGenTextures(1, &m_texture);
//	glBindTexture(GL_TEXTURE_2D, m_texture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, Imagedata);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//	stbi_image_free(Imagedata);
//
//}
//Texture::~Texture(){}