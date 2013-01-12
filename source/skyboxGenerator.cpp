#include "skyboxGenerator.hpp"
#include "skybox.hpp"
#include "texture.hpp"

#include <random>

using namespace std;

namespace Building_Generator
{

void GenerateSkyboxTexture(Texture2D& texture, unsigned int length)
{
    const size_t textureSize = static_cast<size_t>(length*length);

    Texture2D::rgb * textureData = new Texture2D::rgb[textureSize];

    const Texture2D::rgb black = {0, 0, 0};
    const Texture2D::rgb white = {(char)255, (char)255, (char)255};

    int r = 0;
    for(size_t i=0; i< textureSize; ++i)
    {
        r = rand()%1000;
        if(r<5)
            textureData[i] = white;
        else
            textureData[i] = black;
    }

    texture.setTexture(textureData, length, length);
}

Skybox * GenerateSkybox()
{
    const unsigned int cubeLength = 1024;
    Texture2D xPos, xNeg, yPos, yNeg, zPos, zNeg;

    GenerateSkyboxTexture(xPos, cubeLength);
    GenerateSkyboxTexture(xNeg, cubeLength);
    GenerateSkyboxTexture(yPos, cubeLength);
    GenerateSkyboxTexture(yNeg, cubeLength);
    GenerateSkyboxTexture(zPos, cubeLength);
    GenerateSkyboxTexture(zNeg, cubeLength);

    return new Skybox(xPos, xNeg, yPos, yNeg, zPos, zNeg);
}

}
