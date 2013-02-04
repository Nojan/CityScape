#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <vector>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

class ShaderProgram {
public:
    ShaderProgram(GLuint programID);
    ~ShaderProgram();

    GLuint ProgramID() const;
    void UseShaderProgramIFN() const;

    bool IsBind() const;
    void Bind() const;

    //operator
    bool operator== (const ShaderProgram & rhs) const;

private:
    GLuint mProgramID;
    static GLuint mCurrentProgramID;
};

#endif
