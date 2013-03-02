#ifndef STREET_GENERATOR_HPP
#define STREET_GENERATOR_HPP

#include "renderable.hpp"

#include "boundingBox.hpp"
#include "types.hpp"

#include <vector>

namespace Street_Generator
{
    void GenerateStreetScene(const uint width, const uint length, std::vector<RenderableInstance*>& buildingInstances, std::vector<RenderableInstance*>& streetInstances, std::vector<Renderable*>& scene);
}

#endif //STREET_GENERATOR_HPP
