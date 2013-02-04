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

    void GenerateStreetConfiguration(const unsigned int width, const unsigned int length, vector<Area> & streetConfiguration)
    {
        assert( streetConfiguration.empty() );
        const unsigned int size = width*length;
        streetConfiguration.resize(size);

        vector<float> widthSideA;
        vector<float> widthSideB;

        widthSideA.resize(width+1);
        widthSideB.resize(length+1);

        bool isPair = true;
        float sum = 0;
        for(unsigned int i=1; i<(width+1); ++i)
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
        for(unsigned int i=1; i<(length+1); ++i)
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
        for(unsigned int i=0; i<width; ++i )
        {
            const unsigned int iOfsset = i*width;
            bool isColPair = true;
            const float minWidth = widthSideA[i];
            const float maxWidth = widthSideA[i+1];
            for(unsigned int j=0; j<length; ++j)
            {
                const unsigned int index = iOfsset + j;
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

    void GenerateStreetScene(const unsigned int width, const unsigned int length, vector<RenderableInstance*>& buildingInstance, vector<RenderableInstance*>& streetInstance, vector<Renderable*>& scene)
    {
        assert( scene.empty() );
        const unsigned int size = width*length;
        vector<Area> streetConfiguration;
        GenerateStreetConfiguration(width, length, streetConfiguration);
        assert( size == streetConfiguration.size() );
        scene.reserve( size );

        for(unsigned int i=0; i<width; ++i )
        {
            const unsigned int iOfsset = i*width;
            for(unsigned int j=0; j<length; ++j)
            {
                const unsigned int index = iOfsset + j;
                const AreaType type = streetConfiguration[index].type;
                const BoundingBox2D bbox = streetConfiguration[index].bb;
                const vec2 position = bbox.Min();
                mat4 matTransform = translate(mat4(1.f), vec3(position.x, 0.f, position.y));
                Renderable * renderable = new Renderable();
                if( AreaType::building == type )
                {
                    const float height = static_cast<float>(rand()%46+4);
                    RenderableTextureInstance * building = Building_Generator::GenerateBox(bbox.Size().x, bbox.Size().y, height);
                    buildingInstance.push_back(building);
                    matTransform = translate(matTransform, vec3(bbox.Size().x/2.f, 0.f, bbox.Size().y/2.f));
                    renderable->Init(matTransform, building);
                }
                else
                {
                    assert( AreaType::street == type || AreaType::corner == type );
                    RenderableMaterialInstance * street = Building_Generator::GenerateFloor(bbox.Size().x, bbox.Size().y);
                    streetInstance.push_back(street);
                    renderable->Init(matTransform, street);
                }
                scene.push_back(renderable);
            }
        }
        assert( size == scene.size() );
    }
}
