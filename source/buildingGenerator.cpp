#include "buildingGenerator.hpp"

#include "texture.hpp"
#include "color.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

using namespace std;
using namespace glm;

namespace Building_Generator
{

void SetWindowColor(Color::rgb * textureData, Color::hsv color, const size_t buildingWidth, const size_t windowSize, const size_t witdhMargin, const size_t heightMargin )
{
    assert(witdhMargin < (windowSize/2));
    assert(heightMargin < (windowSize/2));

    int horizontalNoiseCount = 0;
    Color::hsv horizontalNoiseColor;

    for(size_t i=heightMargin; i+heightMargin<windowSize; ++i )
    {
        const size_t iOfsset = i*buildingWidth;
        color.v += 0.05f;
        if(color.v < 0.f)
            color.v = 0.f;

        if(horizontalNoiseCount <= 0)
        {
            const int horizontalProbality = rand()%windowSize;
            const int horizontalProbalityThreshold = i+windowSize/2;
            if(horizontalProbality > horizontalProbalityThreshold)
            {
                horizontalNoiseCount = rand()%windowSize;
                horizontalNoiseColor.h = static_cast<float>(rand()%36)/10.f;
                horizontalNoiseColor.s = 0.4f;
            }
        }

        for(size_t j=witdhMargin; j+witdhMargin<windowSize; ++j)
        {
            Color::hsv finalcolor = color;
            if(horizontalNoiseCount >= 0)
            {
                --horizontalNoiseCount;
                finalcolor.h = horizontalNoiseColor.h;
                finalcolor.s += horizontalNoiseColor.s;
            }
            const float noise = static_cast<float>(rand()%10-5)/100.f;
            finalcolor.v += noise;
            finalcolor.h = glm::clamp(finalcolor.h, 0.f, 3.6f);
            finalcolor.s = glm::clamp(finalcolor.s, 0.f, 1.f);
            finalcolor.v = glm::clamp(finalcolor.v, 0.f, 1.f);
            textureData[iOfsset + j] = Color::hsv2rgb(finalcolor);
        }
    }
}

void GenerateBuildingTexture(Texture2D & texture, uint width = 512, uint height = 512, uint windowSize = 8)
    {
        assert(windowSize>2);
        assert(width>=windowSize);
        assert(height>=windowSize);
        const size_t textureSize = static_cast<size_t>(width*height);

        Color::rgb * textureData = new Color::rgb[textureSize];

        const float saturation = static_cast<float>(rand()%20)/100.f;

        const size_t windowWidthMargin = rand()%2+1;
        const size_t windowHeightMargin = rand()%2+1;

        std::vector<float> hueVector =
        {
            3.14f, // blue
            2.20f, // green
            1.50f, // yellow
        };

        const float hue = rand()%hueVector.size();

        const Color::hsv unLit = {0.f, 0.f, 0.f};
        const Color::hsv halfLit = {hue, saturation, 0.2f};
        const Color::hsv fullLit = {hue, saturation, 0.6f};

        const Color::rgb black = {0, 0, 0};
        // Tout en noir
        for(size_t i=0; i< textureSize; ++i)
        {
            textureData[i] = black;
        }

        const int fullLitWindowPercent = 2 + rand()%10;
        const int litWindowPercent = fullLitWindowPercent + 5 + rand()%25;
        const size_t windowPerRow = width/windowSize;
        const size_t windowPerColumn = height/windowSize;
        Color::hsv currentColor = fullLit;
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
                    r = rand()%100;
                    if(r<litWindowPercent)
                    {
                        nb = rand()%6;
                        if(r<fullLitWindowPercent)
                            currentColor = fullLit;
                        else
                            currentColor = halfLit;
                    }
                    else
                        currentColor = unLit;
                }

                if( currentColor.v > 0.f )
                    SetWindowColor(textureData + (rowOffset + column*windowSize), currentColor, width, windowSize, windowWidthMargin, windowHeightMargin );
            }
        }
        texture.setTexture(textureData, width, height);
    }

    class Mesh
    {
    public:
        void Clear();
        void AppendMesh(Mesh const& mesh);
        void Transform(mat3x3 const& rotate);
        void Transform(vec3 const& translate);

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

    void Mesh::Transform(mat3x3 const& rotate)
    {
        const size_t vectorSize = vertexPosition.size();
        for(size_t i=0; i<vectorSize; ++i)
        {
            vertexPosition[i] = rotate * vertexPosition[i];
            vertexNormal[i] = rotate * vertexNormal[i];
        }
    }

    void Mesh::Transform(vec3 const& translate)
    {
        const size_t vectorSize = vertexPosition.size();
        for(size_t i=0; i<vectorSize; ++i)
        {
            vertexPosition[i] = translate + vertexPosition[i];
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

    RenderableMaterialInstance * GenerateFloor(const float width, const float length)
    {
        assert( width>0 && length>0 );
        const float gain = 1.f;
        const float widthf  = static_cast<float>(gain*width);
        const float lengthf = static_cast<float>(gain*length);

        RenderableMaterialInstance * instance = new RenderableMaterialInstance();

        Mesh mesh;
        Mesh subMesh;
        mat4 rotation = mat4(1.f);

        rotation = rotate(rotation, 90.f, vec3(1.f, 0.f, 0.f)); //WTF not rad ?
        CreateQuad(subMesh, widthf, lengthf, vec2(0.f, 0.f), vec2(0.f, 0.f));
        subMesh.Transform(mat3(rotation));
        mesh.AppendMesh(subMesh);

        instance->index.insert(instance->index.end(), mesh.index.begin(), mesh.index.end());
        instance->vertexPosition.insert(instance->vertexPosition.end(), mesh.vertexPosition.begin(), mesh.vertexPosition.end());
        instance->vertexNormal.insert(instance->vertexNormal.end(), mesh.vertexNormal.begin(), mesh.vertexNormal.end());

        return instance;
    }

    RenderableTextureInstance * GenerateBox(const float width, const float length, const float height)
    {
        assert( width>0 && length>0 && height>0 );
        const uint windowSize = 1;
        const uint windowPixelSize = 8;
        const uint texturePixelSize = 512;
        const float textureWindowRatio = static_cast<float>(windowPixelSize)/static_cast<float>(texturePixelSize);
        const float widthf  = static_cast<float>(windowSize*width);
        const float lengthf = static_cast<float>(windowSize*length);
        const float heightf = static_cast<float>(windowSize*height);
        const float widthRatio  = widthf*textureWindowRatio;
        const float lengthRatio = lengthf*textureWindowRatio;
        const float heightRatio = heightf*textureWindowRatio;

        RenderableTextureInstance * instance = new RenderableTextureInstance();

        Mesh mesh;
        Mesh subMesh;
        mat4 rotation = mat4(1.f);
        const vec3 & vecUp = vec3(0.f, 1.f, 0.f);
        float periRatio = widthRatio;
        float lastPeriRatio = 0.f;

        //roof
        rotation = rotate(rotation, -90.f, vec3(1.f, 0.f, 0.f)); //WTF not rad ?
        CreateQuad(subMesh, widthf, lengthf, vec2(0.f, 0.f), vec2(0.f, 0.f));
        subMesh.Transform(mat3(rotation));
        subMesh.Transform(vec3(-widthf*0.5f, heightf, lengthf*0.5f));
        mesh.AppendMesh(subMesh);

        //wall
        rotation = mat4(1.f);
        subMesh.Clear();
        CreateQuad(subMesh, widthf, heightf, vec2(0.f, 0.f), vec2(periRatio, heightRatio));
        lastPeriRatio = periRatio;
        subMesh.Transform(vec3(-widthf*0.5f, 0.f, lengthf*0.5f));
        mesh.AppendMesh(subMesh);

        subMesh.Clear();
        rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
        periRatio+=lengthRatio;
        CreateQuad(subMesh, lengthf, heightf, vec2(lastPeriRatio, 0.f), vec2(periRatio, heightRatio));
        lastPeriRatio = periRatio;
        subMesh.Transform(vec3(-lengthf*0.5f, 0.f, widthf*0.5f));
        subMesh.Transform(mat3(rotation));
        mesh.AppendMesh(subMesh);

        subMesh.Clear();
        rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
        periRatio+=widthRatio;
        CreateQuad(subMesh, widthf, heightf, vec2(lastPeriRatio, 0.f), vec2(periRatio, heightRatio));
        lastPeriRatio = periRatio;
        subMesh.Transform(vec3(-widthf*0.5f, 0.f, lengthf*0.5f));
        subMesh.Transform(mat3(rotation));
        mesh.AppendMesh(subMesh);

        subMesh.Clear();
        rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
        periRatio+=lengthRatio;
        CreateQuad(subMesh, lengthf, heightf, vec2(lastPeriRatio, 0.f), vec2(periRatio, heightRatio));
        lastPeriRatio = periRatio;
        subMesh.Transform(vec3(-lengthf*0.5f, 0.f, widthf*0.5f));
        subMesh.Transform(mat3(rotation));
        mesh.AppendMesh(subMesh);

        instance->index.insert(instance->index.end(), mesh.index.begin(), mesh.index.end());
        instance->vertexPosition.insert(instance->vertexPosition.end(), mesh.vertexPosition.begin(), mesh.vertexPosition.end());
        instance->vertexNormal.insert(instance->vertexNormal.end(), mesh.vertexNormal.begin(), mesh.vertexNormal.end());
        instance->uv.insert(instance->uv.end(), mesh.uv.begin(), mesh.uv.end());

        GenerateBuildingTexture(*(instance->texture), texturePixelSize, texturePixelSize, windowPixelSize);

        return instance;
    }

    RenderableTextureInstance * GenerateLayeredBox(const float width, const float length, const float height, const uint layer, const float layerRatio)
    {
        assert( width>0 && length>0 && height>0 );
        assert( layerRatio>=0 && layerRatio<=1 );
        const uint windowSize = 1;
        const uint windowPixelSize = 8;
        const uint texturePixelSize = 512;
        const float textureWindowRatio = static_cast<float>(windowPixelSize)/static_cast<float>(texturePixelSize);
        const float widthf  = static_cast<float>(windowSize*width);
        const float lengthf = static_cast<float>(windowSize*length);
        const float heightf = static_cast<float>(windowSize*height);
        const float widthRatio  = widthf*textureWindowRatio;
        const float lengthRatio = lengthf*textureWindowRatio;
        const float heightRatio = heightf*textureWindowRatio;

        RenderableTextureInstance * instance = new RenderableTextureInstance();

        Mesh mesh;
        Mesh subMesh;
        mat4 rotation = mat4(1.f);
        const vec3 & vecUp = vec3(0.f, 1.f, 0.f);

        for(uint indexLayer=0; indexLayer<layer; ++indexLayer)
        {
            const float indexLayerf = static_cast<float>(indexLayer);
            const float layerHeight = heightf/static_cast<float>(layer);
            const float layerWidth = widthf*(1.f - indexLayerf*layerRatio);
            const float layerLength = lengthf*(1.f - indexLayerf*layerRatio);
            const vec3 layerBaseHeight = layerHeight*indexLayerf*vecUp;

            //texture coord
            const vec2 coordHeight(heightRatio*indexLayerf/static_cast<float>(layer), heightRatio*(indexLayerf+1.f)/static_cast<float>(layer));

            float periRatio = widthRatio;
            float lastPeriRatio = 0.f;

            //roof
            rotation = rotate(mat4(1.f), -90.f, vec3(1.f, 0.f, 0.f)); //WTF not rad ?
            subMesh.Clear();
            CreateQuad(subMesh, layerWidth, layerLength, vec2(0.f, 0.f), vec2(0.f, 0.f));
            subMesh.Transform(mat3(rotation));
            subMesh.Transform(vec3(-layerWidth*0.5f, layerHeight, layerLength*0.5f) );
            subMesh.Transform(layerBaseHeight);
            mesh.AppendMesh(subMesh);

            //wall
            rotation = mat4(1.f);
            subMesh.Clear();
            CreateQuad(subMesh, layerWidth, layerHeight, vec2(0.f, coordHeight.x), vec2(periRatio, coordHeight.y));
            lastPeriRatio = periRatio;
            subMesh.Transform(vec3(-layerWidth*0.5f, 0.f, layerLength*0.5f));
            subMesh.Transform(layerBaseHeight);
            mesh.AppendMesh(subMesh);

            subMesh.Clear();
            rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
            periRatio+=lengthRatio;
            CreateQuad(subMesh, layerLength, layerHeight, vec2(lastPeriRatio, coordHeight.x), vec2(periRatio, coordHeight.y));
            lastPeriRatio = periRatio;
            subMesh.Transform(vec3(-layerLength*0.5f, 0.f, layerWidth*0.5f));
            subMesh.Transform(mat3(rotation));
            subMesh.Transform(layerBaseHeight);
            mesh.AppendMesh(subMesh);

            subMesh.Clear();
            rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
            periRatio+=widthRatio;
            CreateQuad(subMesh, layerWidth, layerHeight, vec2(lastPeriRatio, coordHeight.x), vec2(periRatio, coordHeight.y));
            lastPeriRatio = periRatio;
            subMesh.Transform(vec3(-layerWidth*0.5f, 0.f, layerLength*0.5f));
            subMesh.Transform(mat3(rotation));
            subMesh.Transform(layerBaseHeight);
            mesh.AppendMesh(subMesh);

            subMesh.Clear();
            rotation = rotate(rotation, 90.f, vecUp); //WTF not rad ?
            periRatio+=lengthRatio;
            CreateQuad(subMesh, layerLength, layerHeight, vec2(lastPeriRatio, coordHeight.x), vec2(periRatio, coordHeight.y));
            lastPeriRatio = periRatio;
            subMesh.Transform(vec3(-layerLength*0.5f, 0.f, layerWidth*0.5f));
            subMesh.Transform(mat3(rotation));
            subMesh.Transform(layerBaseHeight);
            mesh.AppendMesh(subMesh);
        }
        instance->index.insert(instance->index.end(), mesh.index.begin(), mesh.index.end());
        instance->vertexPosition.insert(instance->vertexPosition.end(), mesh.vertexPosition.begin(), mesh.vertexPosition.end());
        instance->vertexNormal.insert(instance->vertexNormal.end(), mesh.vertexNormal.begin(), mesh.vertexNormal.end());
        instance->uv.insert(instance->uv.end(), mesh.uv.begin(), mesh.uv.end());

        GenerateBuildingTexture(*(instance->texture), texturePixelSize, texturePixelSize, windowPixelSize);

        return instance;
    }
}
