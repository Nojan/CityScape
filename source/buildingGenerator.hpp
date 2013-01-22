#ifndef BUILDING_GENERATOR_HPP
#define BUILDING_GENERATOR_HPP

#include "renderableMaterial.hpp"
#include "renderableTexture.hpp"

namespace Building_Generator
{
	RenderableTextureInstance * GenerateBox(unsigned int width, unsigned int length, unsigned int height);
    RenderableMaterialInstance * GenerateFloor(unsigned int width, unsigned int length);
}

#endif //BUILDING_GENERATOR
