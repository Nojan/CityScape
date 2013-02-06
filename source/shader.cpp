#include "shader.hpp"
GLuint ShaderProgram::mCurrentProgramID = 0;

#include "renderer.hpp"

#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
using namespace std;

ShaderProgram::ShaderProgram(GLuint programID)
{
    assert(programID != 0);
    mProgramID = programID;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mProgramID); CHECK_OPENGL_ERROR
}

GLuint ShaderProgram::ProgramID() const
{
    return mProgramID;
}

void ShaderProgram::UseShaderProgramIFN()
{
    if( !IsBind() )
    {
        Bind(); CHECK_OPENGL_ERROR
    }
}

bool ShaderProgram::IsBind() const
{
    return mProgramID == mCurrentProgramID;
}

void ShaderProgram::Bind()
{
    assert(mProgramID != mCurrentProgramID);
    glUseProgram(mProgramID); CHECK_OPENGL_ERROR
    mCurrentProgramID = mProgramID;
}

bool ShaderProgram::operator== (const ShaderProgram & rhs) const
{
    ProgramID() == rhs.ProgramID();
}

//from http://www.opengl-tutorial.org/
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER); CHECK_OPENGL_ERROR
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); CHECK_OPENGL_ERROR

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    assert(VertexShaderStream.is_open());
    {
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    assert(FragmentShaderStream.is_open());
    {
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


