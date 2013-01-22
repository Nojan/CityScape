#ifndef RENDERABLE_MATERIAL_HPP
#define RENDERABLE_MATERIAL_HPP

#include "renderable.hpp"

class RenderableMaterialInstance : public RenderableInstance
{
    struct Material {
        glm::vec4 ambientFactor;
        glm::vec4 diffuseFactor;
        glm::vec4 specularFactor;
        glm::vec4 emissiveValue;
        float shininess;
    };
public:
    RenderableMaterialInstance();
    ~RenderableMaterialInstance();

    void Init(GLuint programID);

    virtual void Draw(const glm::mat4 &model) const;

    virtual void Bind();
    virtual void Unbind();

    GLuint IndexId() const;
    GLuint VertexId() const;
    GLuint NormalId() const;

    std::vector<unsigned short> index;
    std::vector<glm::vec3> vertexPosition;
    std::vector<glm::vec3> vertexNormal;

private:
    GLuint mIndexbuffer;
    GLuint mVertexbuffer;
    GLuint mVertexNormalbuffer;
    GLuint mProgramID;
};

#endif
