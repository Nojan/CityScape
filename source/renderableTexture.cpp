#include "renderableTexture.hpp"

#include "camera.hpp"
#include "root.hpp"

#include "objloader.hpp"
#include "texture.hpp"

#include "glm/gtc/type_ptr.hpp"

using namespace std;

RenderableTextureInstance::RenderableTextureInstance()
: texture(new Texture2D())
{}

RenderableTextureInstance::~RenderableTextureInstance()
{
    delete texture;
}

void RenderableTextureInstance::Init(GLuint programID)
{
    mProgramID = programID;
}

void RenderableTextureInstance::Draw(const glm::mat4 &model) const
{
    assert( IsBind() );

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(mProgramID, "vertexPosition_modelspace");
    GLuint vertexNormal_modelspaceID   = glGetAttribLocation(mProgramID, "vertexNormal_modelspace");
    GLuint vertexUVID                  = glGetAttribLocation(mProgramID, "vertexUV");
    GLuint textureID                   = glGetUniformLocation(mProgramID, "textureSampler");
    GLuint matrixMVP_ID                = glGetUniformLocation(mProgramID, "MVP");
    GLuint matrixMV_ID                 = glGetUniformLocation(mProgramID, "MV");

    glm::mat4 MVP = Root::Instance().GetCamera()->ProjectionView() * model;
    glm::mat4 MV = Root::Instance().GetCamera()->View() * model;

    // Send our transformation to the currently bound shader,
    glUniformMatrix4fv(matrixMVP_ID, 1, GL_FALSE, glm::value_ptr(MVP));
    glUniformMatrix4fv(matrixMV_ID, 1, GL_FALSE, glm::value_ptr(MV));

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureId());
    glUniform1i(textureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VertexId());
    glVertexAttribPointer(
        vertexPosition_modelspaceID, // The attribute we want to configure
        3,                           // size
        GL_FLOAT,                    // type
        GL_FALSE,                    // normalized?
        0,                           // stride
        (void*)0                     // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, UvId());
    glVertexAttribPointer(
        vertexUVID,                   // The attribute we want to configure
        2,                            // size : U+V => 2
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, NormalId());
    glVertexAttribPointer(
        vertexNormal_modelspaceID,    // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_TRUE,                      // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    );

    // 4rd attribute buffer : index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexId());

    // Draw the triangleS !
    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void RenderableTextureInstance::Bind()
{
    RenderableInstance::Bind();

    glGenBuffers(1, &mIndexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned short), index.data() , GL_STATIC_DRAW);

    glGenBuffers(1, &mVertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size()*sizeof(glm::vec3), vertexPosition.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mVertexNormalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexNormalbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexNormal.size()*sizeof(glm::vec3), vertexNormal.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mUvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mUvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size()*sizeof(glm::vec2), uv.data(), GL_STATIC_DRAW);

    glGenTextures(1, &mTexturebuffer);
    glBindTexture(GL_TEXTURE_2D, mTexturebuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->getWidth(), texture->getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, texture->getData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

GLuint RenderableTextureInstance::IndexId() const
{
    assert( IsBind() );
    return mIndexbuffer;
}

GLuint RenderableTextureInstance:: VertexId() const
{
    assert( IsBind() );
    return mVertexbuffer;
}

GLuint RenderableTextureInstance::NormalId() const
{
    assert( IsBind() );
    return mVertexNormalbuffer;
}

GLuint RenderableTextureInstance::UvId() const
{
    assert( IsBind() );
    return mUvbuffer;
}

GLuint RenderableTextureInstance::TextureId() const
{
    assert( IsBind() );
    return mTexturebuffer;
}

void RenderableTextureInstance::Unbind()
{
    RenderableInstance::Unbind();

    glDeleteBuffers(1, &mIndexbuffer);
    glDeleteBuffers(1, &mVertexbuffer);
    glDeleteBuffers(1, &mVertexNormalbuffer);
    glDeleteBuffers(1, &mUvbuffer);
    glDeleteBuffers(1, &mTexturebuffer);
}

RenderableTextureInstance * RenderableTextureInstance::MakeInstanceFrom(char const * pathToObj, char const * pathToTexture)
{
    RenderableTextureInstance * instance = new RenderableTextureInstance();
    loadIndexedOBJ(pathToObj, instance->index, instance->vertexPosition, instance->uv, instance->vertexNormal);
    Texture2D::loadBMP_custom(pathToTexture, *(instance->texture));

    return instance;
}
