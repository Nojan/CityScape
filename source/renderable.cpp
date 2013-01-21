#include "renderable.hpp"

#include "camera.hpp"
#include "root.hpp"

#include "objloader.hpp"
#include "texture.hpp"

#include "glm/gtc/type_ptr.hpp"

using namespace std;

RenderableTextureInstance::RenderableTextureInstance()
: mBind(false)
, texture(new Texture2D())
{}

RenderableTextureInstance::~RenderableTextureInstance()
{
    assert(false == mBind);
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
    assert(false == mBind);

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

    mBind = true;
}

GLuint RenderableTextureInstance::IndexId() const
{
    assert(true == mBind);
    return mIndexbuffer;
}

GLuint RenderableTextureInstance:: VertexId() const
{
    assert(true == mBind);
    return mVertexbuffer;
}

GLuint RenderableTextureInstance::NormalId() const
{
    assert(true == mBind);
    return mVertexNormalbuffer;
}

GLuint RenderableTextureInstance::UvId() const
{
    assert(true == mBind);
    return mUvbuffer;
}

GLuint RenderableTextureInstance::TextureId() const
{
    assert(true == mBind);
    return mTexturebuffer;
}

void RenderableTextureInstance::Unbind()
{
    assert(true == mBind);

    glDeleteBuffers(1, &mIndexbuffer);
    glDeleteBuffers(1, &mVertexbuffer);
    glDeleteBuffers(1, &mVertexNormalbuffer);
    glDeleteBuffers(1, &mUvbuffer);
    glDeleteBuffers(1, &mTexturebuffer);


    mBind = false;
}

bool RenderableTextureInstance::IsBind() const
{
    return mBind;
}

RenderableTextureInstance * RenderableTextureInstance::MakeInstanceFrom(char const * pathToObj, char const * pathToTexture)
{
    RenderableTextureInstance * instance = new RenderableTextureInstance();
    loadIndexedOBJ(pathToObj, instance->index, instance->vertexPosition, instance->uv, instance->vertexNormal);
    Texture2D::loadBMP_custom(pathToTexture, *(instance->texture));

    return instance;
}

Renderable::Renderable()
: mInstance(0)
{}

Renderable::~Renderable()
{}

void Renderable::Init(glm::mat4 const & modelTransformMatrix, RenderableTextureInstance const* instance)
{
    assert(instance);
    mModel = modelTransformMatrix;
    mInstance = instance;
}

void Renderable::Terminate()
{
    mInstance = 0;
}

void Renderable::Draw()
{
    assert(mInstance);

    mInstance->Draw(mModel);
}

void Renderable::DrawDebug(GLuint programID)
{
    glm::mat4 MVP = Root::Instance().GetCamera()->ProjectionView() * mModel;

    const size_t vertexCount = mInstance->vertexPosition.size();
    std::vector<glm::vec3> debugVertexPosition;
    std::vector<glm::vec3> debugVertexColor;

    GLuint debugVertexBuffer, debugVertexColorBuffer;

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint vertexColorID               = glGetAttribLocation(programID, "vertexColor");
    GLuint matrixMVP_ID                = glGetUniformLocation(programID, "MVP");

    // Send our transformation to the currently bound shader,
    glUniformMatrix4fv(matrixMVP_ID, 1, GL_FALSE, &MVP[0][0]);

    if(true)
    {
        debugVertexPosition.reserve(2*vertexCount);
        debugVertexColor.reserve(2*vertexCount);

        for(size_t i=0; i<vertexCount; ++i)
        {
            debugVertexPosition.push_back(mInstance->vertexPosition.at(i));
            debugVertexPosition.push_back(mInstance->vertexPosition.at(i)+mInstance->vertexNormal.at(i));
            debugVertexColor.push_back(glm::vec3(1.f, 0.f, 0.f));
            debugVertexColor.push_back(glm::vec3(0.f, 1.f, 0.f));
        }

        glGenBuffers(1, &debugVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, debugVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 2*vertexCount*sizeof(glm::vec3), debugVertexPosition.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &debugVertexColorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, debugVertexColorBuffer);
        glBufferData(GL_ARRAY_BUFFER, 2*vertexCount*sizeof(glm::vec3), debugVertexColor.data(), GL_STATIC_DRAW);

        // Get a handle for our buffers
        GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
        GLuint vertexColorID               = glGetAttribLocation(programID, "vertexColor");
        GLuint matrixMVP_ID                = glGetUniformLocation(programID, "MVP");

        // Send our transformation to the currently bound shader,
        glUniformMatrix4fv(matrixMVP_ID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, debugVertexBuffer);
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, debugVertexColorBuffer);
        glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_LINES, 0, 2*vertexCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glDeleteBuffers(1, &debugVertexBuffer);
        glDeleteBuffers(1, &debugVertexColorBuffer);

        debugVertexPosition.clear();
        debugVertexColor.clear();
    }
    //-------------------------------------------------------------
    if(false)
    {
        debugVertexColor.reserve(vertexCount);

        for(size_t index = 0; index<vertexCount; ++index)
        {
            debugVertexColor.push_back(glm::vec3(1.f, 1.f, 1.f));
        }

        glGenBuffers(1, &debugVertexColorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, debugVertexColorBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(glm::vec3), debugVertexColor.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, mInstance->VertexId());
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, debugVertexColorBuffer);
        glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glDeleteBuffers(1, &debugVertexColorBuffer);
    }
}
