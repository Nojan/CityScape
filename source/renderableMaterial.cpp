#include "renderableMaterial.hpp"

#include "camera.hpp"
#include "root.hpp"

#include "objloader.hpp"
#include "texture.hpp"

#include "glm/gtc/type_ptr.hpp"

#define CHECK_OPENGL_ERROR \
{ GLenum error; \
  while ( (error = glGetError()) != GL_NO_ERROR) { \
    printf( "OpenGL ERROR: %s\nCHECK POINT: %s (line %d)\n", gluErrorString(error), __FILE__, __LINE__ ); \
    assert(error == GL_NO_ERROR); \
  } \
}

using namespace std;

RenderableMaterialInstance::RenderableMaterialInstance()
{
}

RenderableMaterialInstance::~RenderableMaterialInstance()
{
}

void RenderableMaterialInstance::Draw(const glm::mat4 &model) const
{
    assert( 0 != ProgramID() );
    glUseProgram( ProgramID() );
    assert( IsBind() );
    CHECK_OPENGL_ERROR

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(ProgramID(), "vertexPosition_modelspace"); CHECK_OPENGL_ERROR
    GLuint vertexNormal_modelspaceID   = glGetAttribLocation(ProgramID(), "vertexNormal_modelspace"); CHECK_OPENGL_ERROR
    GLuint material_ambientFactor_ID   = glGetUniformLocation(ProgramID(), "material.ambientFactor"); CHECK_OPENGL_ERROR
    GLuint material_diffuseFactor_ID   = glGetUniformLocation(ProgramID(), "material.diffuseFactor"); CHECK_OPENGL_ERROR
    GLuint material_specularFactor_ID  = glGetUniformLocation(ProgramID(), "material.specularFactor"); CHECK_OPENGL_ERROR
    GLuint material_emissiveValue_ID   = glGetUniformLocation(ProgramID(), "material.emissiveValue"); CHECK_OPENGL_ERROR
    GLuint material_shininess_ID       = glGetUniformLocation(ProgramID(), "material.shininess"); CHECK_OPENGL_ERROR
    GLuint matrixMVP_ID                = glGetUniformLocation(ProgramID(), "MVP"); CHECK_OPENGL_ERROR
    GLuint matrixMV_ID                 = glGetUniformLocation(ProgramID(), "MV"); CHECK_OPENGL_ERROR

    glm::mat4 MVP = Root::Instance().GetCamera()->ProjectionView() * model;
    glm::mat4 MV = Root::Instance().GetCamera()->View() * model;

    Material mat;
    mat.ambientFactor = glm::vec4(0.f);
    mat.diffuseFactor = glm::vec4(0.f);
    mat.specularFactor = glm::vec4(0.f);
    mat.emissiveValue = glm::vec4(0.05f, 0.05f, 0.05f, 1.f);
    mat.shininess = 0.f;

    // Send our transformation to the currently bound shader,
    glUniformMatrix4fv(matrixMVP_ID, 1, GL_FALSE, glm::value_ptr(MVP)); CHECK_OPENGL_ERROR
    glUniformMatrix4fv(matrixMV_ID, 1, GL_FALSE, glm::value_ptr(MV)); CHECK_OPENGL_ERROR
    glUniform4fv(material_ambientFactor_ID, 1, glm::value_ptr(mat.ambientFactor)); CHECK_OPENGL_ERROR
    glUniform4fv(material_diffuseFactor_ID, 1, glm::value_ptr(mat.diffuseFactor)); CHECK_OPENGL_ERROR
    glUniform4fv(material_specularFactor_ID, 1, glm::value_ptr(mat.specularFactor)); CHECK_OPENGL_ERROR
    glUniform4fv(material_emissiveValue_ID, 1, glm::value_ptr(mat.emissiveValue)); CHECK_OPENGL_ERROR
    glUniform1f(material_shininess_ID, mat.shininess); CHECK_OPENGL_ERROR

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0); CHECK_OPENGL_ERROR
    glBindBuffer(GL_ARRAY_BUFFER, VertexId()); CHECK_OPENGL_ERROR
    glVertexAttribPointer(
        vertexPosition_modelspaceID, // The attribute we want to configure
        3,                           // size
        GL_FLOAT,                    // type
        GL_FALSE,                    // normalized?
        0,                           // stride
        (void*)0                     // array buffer offset
    ); CHECK_OPENGL_ERROR

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1); CHECK_OPENGL_ERROR
    glBindBuffer(GL_ARRAY_BUFFER, NormalId()); CHECK_OPENGL_ERROR
    glVertexAttribPointer(
        vertexNormal_modelspaceID,    // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_TRUE,                      // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    ); CHECK_OPENGL_ERROR

    // 3rd attribute buffer : index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexId()); CHECK_OPENGL_ERROR

    // Draw the triangleS !
    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_SHORT, (void*)0); CHECK_OPENGL_ERROR

    glDisableVertexAttribArray(0); CHECK_OPENGL_ERROR
    glDisableVertexAttribArray(1); CHECK_OPENGL_ERROR
}

void RenderableMaterialInstance::Bind()
{
    RenderableInstance::Bind(); CHECK_OPENGL_ERROR

    glGenBuffers(1, &mIndexbuffer); CHECK_OPENGL_ERROR
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexbuffer); CHECK_OPENGL_ERROR
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned short), index.data() , GL_STATIC_DRAW); CHECK_OPENGL_ERROR

    glGenBuffers(1, &mVertexbuffer); CHECK_OPENGL_ERROR
    glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer); CHECK_OPENGL_ERROR
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size()*sizeof(glm::vec3), vertexPosition.data(), GL_STATIC_DRAW); CHECK_OPENGL_ERROR

    glGenBuffers(1, &mVertexNormalbuffer); CHECK_OPENGL_ERROR
    glBindBuffer(GL_ARRAY_BUFFER, mVertexNormalbuffer); CHECK_OPENGL_ERROR
    glBufferData(GL_ARRAY_BUFFER, vertexNormal.size()*sizeof(glm::vec3), vertexNormal.data(), GL_STATIC_DRAW); CHECK_OPENGL_ERROR
}

GLuint RenderableMaterialInstance::IndexId() const
{
    assert( IsBind() );
    return mIndexbuffer;
}

GLuint RenderableMaterialInstance:: VertexId() const
{
    assert( IsBind() );
    return mVertexbuffer;
}

GLuint RenderableMaterialInstance::NormalId() const
{
    assert( IsBind() );
    return mVertexNormalbuffer;
}

void RenderableMaterialInstance::Unbind()
{
    RenderableInstance::Unbind();

    glDeleteBuffers(1, &mIndexbuffer); CHECK_OPENGL_ERROR
    glDeleteBuffers(1, &mVertexbuffer); CHECK_OPENGL_ERROR
    glDeleteBuffers(1, &mVertexNormalbuffer); CHECK_OPENGL_ERROR
}

