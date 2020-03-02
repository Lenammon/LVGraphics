#include "Shader.h"
Shader::Shader(std::string vertex, std::string frag)
{
	//vertex shader
	std::string shader_data;
	std::ifstream in_file_stream(vertex, std::ifstream::in);

	std::stringstream string_stream;
	string_stream.clear();
	if (in_file_stream.is_open())
	{
		string_stream << in_file_stream.rdbuf();
		shader_data = string_stream.str();
		in_file_stream.close();
	}

	////-------------------------------------------------------------
	//allocate space for shader program 
	vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
	//convert to raw char
	const char* data = shader_data.c_str();
	//send in the chr* to shader location 
	glShaderSource(vertex_shader_ID, 1, (const GLchar**)&data, 0);
	//build
	glCompileShader(vertex_shader_ID);
	
	ErrorCheckVertexShader();


	//fragment shader------------------------------------------------------------------------


	//std::string shader_data;
	std::ifstream in_file_stream_frag(frag, std::ifstream::in);

	std::stringstream string_stream_frag;
	if (in_file_stream_frag.is_open())
	{
		string_stream_frag << in_file_stream_frag.rdbuf();
		shader_data = string_stream_frag.str();
		in_file_stream_frag.close();
	}
	//allocate space for shader program 
	fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
	//convert to raw char
	//const char* 
	data = shader_data.c_str();
	//send in the chr* to shader location 
	glShaderSource(fragment_shader_ID, 1, (const GLchar**)&data, 0);
	//build
	glCompileShader(fragment_shader_ID);
	////error checking 
	ErrorCheckFragShader();
	

	ErrorCheckLinker();
	

	

}
Shader::~Shader()
{

}

void Shader::ErrorCheckVertexShader()// if the shaderdoes not complie this will print out the erros in the console 
{
	//GLint succuess = GL_FALSE; // this needs to be private?
	//error checking  vertex shader
	GLint success = GL_FALSE;
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		printf("vertex shader fail");
		GLint log_lenth = 0;
		glGetShaderiv(vertex_shader_ID, GL_INFO_LOG_LENGTH, &log_lenth);
		//create error buffer
		char* log = new char[log_lenth];
		//copy the error buffer
		glGetShaderInfoLog(vertex_shader_ID, log_lenth, 0, log);
		//create the error message 
		std::string error_message(log);
		error_message += "shader failed to compile";
		printf(error_message.c_str());
		//clean up
		delete[] log;
	}

}
void Shader::ErrorCheckFragShader()
{

	//error checking 
	GLint success = GL_FALSE;
	glGetShaderiv(fragment_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		printf("fragment shader fail");
		GLint log_lenth = 0;
		glGetShaderiv(fragment_shader_ID, GL_INFO_LOG_LENGTH, &log_lenth);
		//create error buffer
		char* log = new char[log_lenth];
		//copy the error buffer
		glGetShaderInfoLog(fragment_shader_ID, log_lenth, 0, log);
		//create the error message 
		std::string error_message(log);
		error_message += "shader failed to compile";
		printf(error_message.c_str());
		//clean up
		delete[] log;
	}
	

}
void Shader::ErrorCheckLinker()
{
	//link 
	shader_program_ID = glCreateProgram();
	glAttachShader(shader_program_ID, vertex_shader_ID);
	glAttachShader(shader_program_ID, fragment_shader_ID);


	glLinkProgram(shader_program_ID);
	success = GL_FALSE;
	glGetProgramiv(shader_program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		printf(" link fail");
		GLint log_lenth = 0;
		glGetProgramiv(shader_program_ID, GL_INFO_LOG_LENGTH, &log_lenth);
		//create error buffer
		char* log = new char[log_lenth];
		//copy the error buffer
		glGetProgramInfoLog(shader_program_ID, log_lenth, 0, log);
		//create the error message 
		std::string error_message(log);
		error_message += "shader failed to compile";
		printf(error_message.c_str());
		//clean up
		delete[] log;
	}
}



void Shader::BindShader()
{
	//bind the shader 
	//bind the light direction 
	//bind th PVM
	//bind the normal matrix
	glUseProgram(shader_program_ID);
}