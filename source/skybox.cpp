#include "skybox.hpp"

#include "camera.hpp"
#include "root.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include "glm/gtc/type_ptr.hpp"

using namespace std;

Skybox* Skybox::CreateSkyboxFrom(const char  * xPosPath, const char  * xNegPath,
                                 const char  * yPosPath, const char  * yNegPath,
                                 const char  * zPosPath, const char  * zNegPath)
{
    Texture2D xPos, xNeg, yPos, yNeg, zPos, zNeg;

    Texture2D::loadBMP_custom(xPosPath, xPos);
    Texture2D::loadBMP_custom(xNegPath, xNeg);
    Texture2D::loadBMP_custom(yPosPath, yPos);
    Texture2D::loadBMP_custom(yNegPath, yNeg);
    Texture2D::loadBMP_custom(zPosPath, zPos);
    Texture2D::loadBMP_custom(zNegPath, zNeg);

    return new Skybox(xPos, xNeg, yPos, yNeg, zPos, zNeg);
}

Skybox::Skybox(Texture2D& xPos, Texture2D& xNeg, Texture2D& yPos, Texture2D& yNeg, Texture2D& zPos, Texture2D& zNeg)
{
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &mTextureBufferId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureBufferId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, xPos.getWidth(), xPos.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, xPos.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, xNeg.getWidth(), xNeg.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, xNeg.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, yPos.getWidth(), yPos.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, yPos.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, yNeg.getWidth(), yNeg.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, yNeg.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, zPos.getWidth(), zPos.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, zPos.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, zNeg.getWidth(), zNeg.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, zNeg.getData());

    GLfloat cube_vertices[] = {
      -1.f,  1.f,  1.f,
      -1.f, -1.f,  1.f,
       1.f, -1.f,  1.f,
       1.f,  1.f,  1.f,
      -1.f,  1.f, -1.f,
      -1.f, -1.f, -1.f,
       1.f, -1.f, -1.f,
       1.f,  1.f, -1.f,
    };
    glGenBuffers(1, &mVertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    GLushort cube_indices[] = {
      0, 1, 2, 3,
      3, 2, 6, 7,
      7, 6, 5, 4,
      4, 5, 1, 0,
      0, 3, 7, 4,
      1, 2, 6, 5,
    };
    glGenBuffers(1, &mIndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &mTextureBufferId);
    glDeleteBuffers(1, &mIndexBufferId);
    glDeleteBuffers(1, &mVertexBufferId);
}

void Skybox::Draw(ShaderProgram * programShader)
{
    assert(NULL != programShader);
    programShader->UseShaderProgramIFN();

    // Get a handle for our buffers
    GLuint vertexPositionID = glGetAttribLocation(programShader->ProgramID(), "vertexPosition");
    GLuint cubemapID        = glGetUniformLocation(programShader->ProgramID(), "cubemapSampler");
    GLuint matrixMVP_ID     = glGetUniformLocation(programShader->ProgramID(), "MVP");

    glm::mat4 model = glm::scale(glm::mat4(1.0f),glm::vec3(100,100,100));
    const Camera * camera = Root::Instance().GetCamera();
    glm::mat4 view = camera->View();
    view[3][0] = 0; view[3][1] = 0; view[3][2] = 0; // Pas de translation pour la skybox
    glm::mat4 MVP = Root::Instance().GetCamera()->Projection() * view * model;
    glUniformMatrix4fv(matrixMVP_ID, 1, GL_FALSE, glm::value_ptr(MVP));

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureBufferId);
    glUniform1i(cubemapID, 0);

    // attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
    glVertexAttribPointer(
        vertexPositionID, // The attribute we want to configure
        3,                // size
        GL_FLOAT,         // type
        GL_FALSE,         // normalized?
        0,                // stride
        (void*)0          // array buffer offset
    );

    // attribute buffer : index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);

    glDrawElements(GL_QUADS, 6*4, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(0);
    glDisable(GL_TEXTURE_CUBE_MAP);
}


