#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <vector>

class Renderable {
public:

    struct Data {
        GLuint vertexbuffer;
        GLuint vertexNormalbuffer;
        GLuint texture;
        GLuint uvbuffer;

        std::vector<glm::vec3> vertexPosition;
        std::vector<glm::vec3> vertexNormal;
        std::vector<glm::vec2> uv;
    };
    static Data const* MakeDataFrom(char const * pathToObj, char const * pathToTexture);

    Renderable();
    ~Renderable();

    void Init(glm::mat4 const& modelTransformMatrix, Data const* data);
    void Terminate();
    void Draw(GLuint programID);
    void DrawDebug(GLuint programID);

private:
    glm::mat4 mModel;

    Data const* mData;
};

#endif
