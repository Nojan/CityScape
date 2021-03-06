#include "streetGenerator.hpp"

#include "buildingGenerator.hpp"
#include "renderableTexture.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

using namespace std;
using namespace glm;

namespace Street_Generator
{
    enum AreaType
    {
        street,
        corner,
        building,
    };

    struct Area
    {
        AreaType type;
        BoundingBox2D bb;
    };

    void GenerateStreetConfiguration(const uint width, const uint length, vector<Area> & streetConfiguration)
    {
        assert( streetConfiguration.empty() );
        const uint size = width*length;
        streetConfiguration.resize(size);

        vector<float> widthSideA;
        vector<float> widthSideB;

        widthSideA.resize(width+1);
        widthSideB.resize(length+1);

        bool isPair = true;
        float sum = 0;
        for(uint i=1; i<(width+1); ++i)
        {
            if(isPair)
                sum += static_cast<float>(rand()%2+2);
            else
                sum += static_cast<float>(rand()%30+10);
            widthSideA[i] = sum;
            assert( widthSideA[i-1] < widthSideA[i]);
            isPair = !isPair;
        }

        isPair = true;
        sum = 0;
        for(uint i=1; i<(length+1); ++i)
        {
            if(isPair)
                sum += static_cast<float>(rand()%2+2);
            else
                sum += static_cast<float>(rand()%30+10);
            widthSideB[i] = sum;
            assert( widthSideB[i-1] < widthSideB[i]);
            isPair = !isPair;
        }

        bool isLinePair = true;
        for(uint i=0; i<width; ++i )
        {
            const uint iOfsset = i*width;
            bool isColPair = true;
            const float minWidth = widthSideA[i];
            const float maxWidth = widthSideA[i+1];
            for(uint j=0; j<length; ++j)
            {
                const uint index = iOfsset + j;
                const float minLength = widthSideB[j];
                const float maxLength = widthSideB[j+1];

                assert( minWidth < maxWidth );
                assert( minLength < maxLength );
                BoundingBox2D bbox = BoundingBox2D(minWidth, minLength, maxWidth, maxLength);
                assert( bbox.Size().x > 0.f );
                assert( bbox.Size().y > 0.f );
                streetConfiguration[index].bb = bbox;

                if(isLinePair)
                {
                    if(isColPair)
                    {
                        streetConfiguration[index].type = AreaType::corner;
                    }
                    else
                    {
                        streetConfiguration[index].type = AreaType::street;
                    }
                }
                else
                {
                    if(isColPair)
                    {
                        streetConfiguration[index].type = AreaType::street;
                    }
                    else
                    {
                        streetConfiguration[index].type = AreaType::building;
                    }
                }
                isColPair = !isColPair;
            }
            isLinePair = !isLinePair;
        }
    }

    void GenerateStreetScene(const uint width, const uint length, vector<RenderableInstance*>& buildingInstances, vector<RenderableInstance*>& streetInstances, vector<Renderable*>& scene)
    {
        assert( scene.empty() );
        const uint size = width*length;
        vector<Area> streetConfiguration;
        GenerateStreetConfiguration(width, length, streetConfiguration);
        assert( size == streetConfiguration.size() );
        scene.reserve( size );

        Renderable * street = new Renderable();
        RenderableMaterialInstance * streetInstance = new RenderableMaterialInstance();

        for(uint i=0; i<width; ++i )
        {
            const uint iOfsset = i*width;
            for(uint j=0; j<length; ++j)
            {
                const uint index = iOfsset + j;
                const AreaType type = streetConfiguration[index].type;
                const BoundingBox2D bbox = streetConfiguration[index].bb;
                const vec2 position = bbox.Min();
                mat4 matTransform = translate(mat4(1.f), vec3(position.x, 0.f, position.y));

                if( AreaType::building == type )
                {
                    Renderable * renderable = new Renderable();
                    RenderableTextureInstance * building = NULL;
                    if(1 < rand()%10)
                    {
                        const float height = static_cast<float>(rand()%20+5);
                        building = Building_Generator::GenerateBox(bbox.Size().x, bbox.Size().y, height);
                    }
                    else
                    {
                        const float height = static_cast<float>(rand()%25+30);
                        const uint layerCount = 2+rand()%3;
                        const float layerRatio = static_cast<float>(rand()%4)*0.1f;
                        building = Building_Generator::GenerateLayeredBox(bbox.Size().x, bbox.Size().y, height, layerCount, layerRatio);
                    }
                    assert(NULL != building);
                    buildingInstances.push_back(building);
                    matTransform = translate(matTransform, vec3(bbox.Size().x/2.f, 0.f, bbox.Size().y/2.f));
                    renderable->Init(matTransform, building);
                    scene.push_back(renderable);
                }
                else
                {
                    assert( AreaType::street == type || AreaType::corner == type );
                    RenderableMaterialInstance * s = Building_Generator::GenerateFloor(bbox.Size().x, bbox.Size().y);
                    // merge instance
                    const size_t offsetIndex = streetInstance->vertexPosition.size();
                    for(size_t index=0; index<s->vertexPosition.size(); ++index)
                    {
                        const vec4 position =  matTransform * vec4(s->vertexPosition.at(index), 1);
                        streetInstance->vertexPosition.push_back(vec3(position));
                        streetInstance->vertexNormal.push_back(s->vertexNormal.at(index));
                    }
                    for(size_t index=0; index<s->index.size(); ++index)
                    {
                        streetInstance->index.push_back(offsetIndex + s->index.at(index));
                        assert(offsetIndex + s->index.at(index) < streetInstance->vertexNormal.size());
                        assert( streetInstance->vertexNormal.at( offsetIndex + s->index.at(index) ) == s->vertexNormal.at(s->index.at(index)));
                    }
                }
            }
        }
        street->Init(mat4(1.f), streetInstance);
        streetInstances.push_back(streetInstance);
        scene.push_back(street);
    }
}
