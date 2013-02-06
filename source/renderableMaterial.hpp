#ifndef RENDERABLE_MATERIAL_HPP
#define RENDERABLE_MATERIAL_HPP

#include "renderable.hpp"
#include "shader.hpp"

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
};


class MaterialShaderProgram : public ShaderProgram
{
public:
    MaterialShaderProgram(GLuint programID);

    virtual void Bind();

    GLuint VertexPosition_modelspaceID() const { return mVertexPosition_modelspaceID; }
    GLuint VertexNormal_modelspaceID() const { return mVertexNormal_modelspaceID; }
    GLuint Material_ambientFactor_ID() const { return mMaterial_ambientFactor_ID; }
    GLuint Material_diffuseFactor_ID() const { return mMaterial_diffuseFactor_ID; }
    GLuint Material_specularFactor_ID() const { return mMaterial_specularFactor_ID; }
    GLuint Material_emissiveValue_ID() const { return mMaterial_emissiveValue_ID; }
    GLuint Material_shininess_ID() const { return mMaterial_shininess_ID; }
    GLuint MatrixMVP_ID() const { return mMatrixMVP_ID; }
    GLuint MatrixMV_ID() const { return mMatrixMV_ID; }

private:
    GLuint mVertexPosition_modelspaceID;
    GLuint mVertexNormal_modelspaceID;
    GLuint mMaterial_ambientFactor_ID;
    GLuint mMaterial_diffuseFactor_ID;
    GLuint mMaterial_specularFactor_ID;
    GLuint mMaterial_emissiveValue_ID;
    GLuint mMaterial_shininess_ID;
    GLuint mMatrixMVP_ID;
    GLuint mMatrixMV_ID;
};


#endif
