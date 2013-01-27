#ifndef STREET_GENERATOR_HPP
#define STREET_GENERATOR_HPP

#include "renderable.hpp"

#include "boundingBox.hpp"

#include <vector>

namespace Street_Generator
{
    void GenerateStreetScene(const unsigned int width, const unsigned int length, std::vector<RenderableInstance*>& buildingInstance, std::vector<RenderableInstance*>& streetInstance, std::vector<Renderable>& scene);
}

#endif //STREET_GENERATOR_HPP
