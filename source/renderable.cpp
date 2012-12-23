#include "renderable.hpp"

#include "camera.hpp"
#include "root.hpp"

#include "objloader.hpp"
#include "texture.hpp"

using namespace std;

Renderable::Data const* Renderable::MakeDataFrom(char const * pathToObj, char const * pathToTexture)
{
    Renderable::Data * data = new Renderable::Data();
    loadIndexedOBJ(pathToObj, data->index, data->vertexPosition, data->uv, data->vertexNormal);
    data->texture = loadBMP_custom(pathToTexture);

    glGenBuffers(1, &(data->indexbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->indexbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->index.size() * sizeof(unsigned short), data->index.data() , GL_STATIC_DRAW);

    glGenBuffers(1, &(data->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, data->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, data->vertexPosition.size()*sizeof(glm::vec3), data->vertexPosition.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &(data->vertexNormalbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, data->vertexNormalbuffer);
    glBufferData(GL_ARRAY_BUFFER, data->vertexNormal.size()*sizeof(glm::vec3), data->vertexNormal.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &(data->uvbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, data->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, data->uv.size()*sizeof(glm::vec2), data->uv.data(), GL_STATIC_DRAW);

    return data;
}

Renderable::Renderable()
: mData(0)
{}

Renderable::~Renderable()
{}

void Renderable::Init(glm::mat4 const & modelTransformMatrix, Data const* data)
{
    assert(data);
    mModel = modelTransformMatrix;
    mData = data;
}

void Renderable::Terminate()
{
    // Cleanup VBO in mData
    //glDeleteBuffers(1, &indexbuffer);
    //glDeleteBuffers(1, &vertexbuffer);
    //glDeleteBuffers(1, &vertexNormalbuffer);
    //glDeleteBuffers(1, &uvbuffer);
    mData = 0;
}

void Renderable::Draw(GLuint programID)
{
    assert(mData);

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint vertexNormal_modelspaceID   = glGetAttribLocation(programID, "vertexNormal_modelspace");
    GLuint vertexUVID                  = glGetAttribLocation(programID, "vertexUV");
    GLuint textureID                   = glGetUniformLocation(programID, "textureSampler");
    GLuint matrixMVP_ID                = glGetUniformLocation(programID, "MVP");
    GLuint matrixM_ID                  = glGetUniformLocation(programID, "M");
    GLuint matrixV_ID                  = glGetUniformLocation(programID, "V");

    glm::mat4 MVP = Root::Instance().GetCamera()->ProjectionView() * mModel;
    glm::mat4 M = mModel;
    glm::mat4 V = Root::Instance().GetCamera()->View();

    // Send our transformation to the currently bound shader,
    glUniformMatrix4fv(matrixMVP_ID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(matrixM_ID, 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(matrixV_ID, 1, GL_FALSE, &V[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mData->texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(textureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mData->vertexbuffer);
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
    glBindBuffer(GL_ARRAY_BUFFER, mData->uvbuffer);
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
    glBindBuffer(GL_ARRAY_BUFFER, mData->vertexNormalbuffer);
    glVertexAttribPointer(
        vertexNormal_modelspaceID,    // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_TRUE,                      // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
    );

    // 4rd attribute buffer : index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mData->indexbuffer);

    // Draw the triangleS !
    glDrawElements(GL_TRIANGLES, mData->index.size(), GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Renderable::DrawDebug(GLuint programID)
{
    glm::mat4 MVP = Root::Instance().GetCamera()->ProjectionView() * mModel;

    const size_t vertexCount = mData->vertexPosition.size();
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
            debugVertexPosition.push_back(mData->vertexPosition.at(i));
            debugVertexPosition.push_back(mData->vertexPosition.at(i)+mData->vertexNormal.at(i));
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
        glBindBuffer(GL_ARRAY_BUFFER, mData->vertexbuffer);
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
