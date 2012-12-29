#include "buildingGenerator.hpp"

#include "texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

using namespace std;
using namespace glm;

namespace Building_Generator
{

void SetWindowColor(Texture2D::rgb * textureData, Texture2D::rgb color, size_t width, size_t windowSize )
{
    for(size_t i=1; i+1<windowSize; ++i )
    {
        const size_t iOfsset = i*width;
        for(size_t j=1; j+1<windowSize; ++j)
        {
            textureData[iOfsset + j] = color;
        }
    }
}

void GenerateBuildingTexture(Texture2D & texture, unsigned int width = 512, unsigned int height = 512, unsigned int windowSize = 8)
    {
        assert(windowSize>2);
        assert(width>=windowSize);
        assert(width>=windowSize);
        const size_t textureSize = static_cast<size_t>(width*height);

        Texture2D::rgb * textureData = new Texture2D::rgb[textureSize];

        const Texture2D::rgb black = {0, 0, 0};
        const Texture2D::rgb grey  = {(char)128, (char)128, (char)128};
        const Texture2D::rgb white = {(char)255, (char)255, (char)255};
        // Tout en noir
        for(size_t i=0; i< textureSize; ++i)
        {
            textureData[i] = black;
        }

        const size_t windowPerRow = width/windowSize;
        const size_t windowPerColumn = height/windowSize;
        Texture2D::rgb currentColor = white;
        for(size_t row = 0; row<windowPerColumn; row++)
        {
            const size_t rowOffset = row*windowSize*width;
            int r = 0;
            int nb = 0;
            for(size_t column = 0; column<windowPerRow; column++)
            {
                if(nb>0)
                {
                    nb--;
                }
                else
                {
                    r = rand()%10;
                    if(r<2)
                    {
                        nb = rand()%6;
                        if(r==0)
                            currentColor = white;
                        else
                            currentColor = grey;
                    }
                    else
                        currentColor = black;
                }

                SetWindowColor(textureData + (rowOffset + column*windowSize), currentColor, width, windowSize );
            }
        }
        texture.setTexture(textureData, width, height);
    }

    class Mesh
    {
    public:
        void Clear();
        void AppendMesh(Mesh const& mesh);
        void Transform(mat3x3 const& rotate, vec3 const& translate);

        std::vector<unsigned short> index;
        std::vector<glm::vec3> vertexPosition;
        std::vector<glm::vec3> vertexNormal;
        std::vector<glm::vec2> uv;
    };

    void Mesh::Clear()
    {
        index.clear();
        vertexPosition.clear();
        vertexNormal.clear();
        uv.clear();
    }

    void Mesh::Transform(mat3x3 const& rotate, vec3 const& translate)
    {
        const size_t vectorSize = vertexPosition.size();
        for(size_t i=0; i<vectorSize; ++i)
        {
            vertexPosition[i] = rotate * (translate + vertexPosition[i]);
            vertexNormal[i] = rotate * vertexNormal[i];
        }
    }

    void Mesh::AppendMesh(Mesh const& mesh)
    {
        const size_t startIndex = vertexPosition.size();
        for(size_t i=0; i<mesh.index.size(); ++i)
        {
            index.push_back(mesh.index[i] + startIndex);
        }
        vertexPosition.insert(vertexPosition.end(), mesh.vertexPosition.begin(), mesh.vertexPosition.end());
        vertexNormal.insert(vertexNormal.end(), mesh.vertexNormal.begin(), mesh.vertexNormal.end());
        uv.insert(uv.end(), mesh.uv.begin(), mesh.uv.end());
    }


    void CreateQuad(Mesh & mesh, float width, float height, vec2 minUV, vec2 maxUV)
    {
        assert(mesh.index.empty());
        assert(mesh.vertexPosition.empty());
        assert(mesh.vertexNormal.empty());
        assert(mesh.uv.empty());

        mesh.index.reserve(6);
        mesh.vertexPosition.reserve(4);
        mesh.vertexNormal.reserve(4);

        mesh.index.push_back(0);
        mesh.index.push_back(1);
        mesh.index.push_back(2);
        mesh.index.push_back(0);
        mesh.index.push_back(2);
        mesh.index.push_back(3);

        mesh.vertexPosition.push_back(vec3(0.f, 0.f, 0.f));
        mesh.vertexPosition.push_back(vec3(0.f, height, 0.f));
        mesh.vertexPosition.push_back(vec3(width, height, 0.f));
        mesh.vertexPosition.push_back(vec3(width, 0.f, 0.f));

        mesh.vertexNormal.push_back(vec3(0.f, 0.f, 1.f));
        mesh.vertexNormal.push_back(vec3(0.f, 0.f, 1.f));
        mesh.vertexNormal.push_back(vec3(0.f, 0.f, 1.f));
        mesh.vertexNormal.push_back(vec3(0.f, 0.f, 1.f));

        mesh.uv.push_back(minUV);
        mesh.uv.push_back(vec2(minUV.x, maxUV.y));
        mesh.uv.push_back(maxUV);
        mesh.uv.push_back(vec2(maxUV.x, minUV.y));
    }

    const unsigned int windowSize = 1;

    RenderableInstance * GenerateBox(unsigned int width, unsigned int length, unsigned int height)
    {
        assert( width>0 && length>0 && height>0 );

        const float widthf  = static_cast<float>(windowSize*width);
        const float lengthf = static_cast<float>(windowSize*length);
        const float heightf = static_cast<float>(windowSize*height);

        RenderableInstance * instance = new RenderableInstance();

        Mesh mesh;
        Mesh subMesh;
        mat4 rotation = mat4(1.f);
        const vec3 & vecUp = vec3(0.f, 1.f, 0.f);

        CreateQuad(subMesh, widthf, heightf, vec2(0.f, 0.f), vec2(0.25f, 0.25f));
        subMesh.Transform(mat3(rotation), vec3(-widthf*0.5f, 0.f, lengthf*0.5f));
        mesh.AppendMesh(subMesh);

        subMesh.Clear();
        rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
        CreateQuad(subMesh, lengthf, heightf, vec2(0.25f, 0.f), vec2(0.5f, 0.25f));
        subMesh.Transform(mat3(rotation), vec3(-lengthf*0.5f, 0.f, widthf*0.5f));
        mesh.AppendMesh(subMesh);

        subMesh.Clear();
        rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
        CreateQuad(subMesh, widthf, heightf, vec2(0.5f, 0.f), vec2(0.75f, 0.25f));
        subMesh.Transform(mat3(rotation), vec3(-widthf*0.5f, 0.f, lengthf*0.5f));
        mesh.AppendMesh(subMesh);

        subMesh.Clear();
        rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
        CreateQuad(subMesh, lengthf, heightf, vec2(0.75f, 0.f), vec2(1.f, 0.25f));
        subMesh.Transform(mat3(rotation), vec3(-lengthf*0.5f, 0.f, widthf*0.5f));
        mesh.AppendMesh(subMesh);

        instance->index.insert(instance->index.end(), mesh.index.begin(), mesh.index.end());
        instance->vertexPosition.insert(instance->vertexPosition.end(), mesh.vertexPosition.begin(), mesh.vertexPosition.end());
        instance->vertexNormal.insert(instance->vertexNormal.end(), mesh.vertexNormal.begin(), mesh.vertexNormal.end());
        instance->uv.insert(instance->uv.end(), mesh.uv.begin(), mesh.uv.end());

        GenerateBuildingTexture(*(instance->texture));

        return instance;
    }
}
