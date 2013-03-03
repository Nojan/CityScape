#ifndef BUILDING_GENERATOR_HPP
#define BUILDING_GENERATOR_HPP

#include "renderableMaterial.hpp"
#include "renderableTexture.hpp"

#include "types.hpp"

namespace Building_Generator
{
    RenderableTextureInstance * GenerateBox(const float width, const float length, const float height);
    RenderableTextureInstance * GenerateLayeredBox(const float width, const float length, const float height, const uint layer, const float layerRatio);
    RenderableMaterialInstance * GenerateFloor(const float width, const float length);
}

#endif //BUILDING_GENERATOR
