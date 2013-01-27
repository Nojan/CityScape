#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include <glm/glm.hpp>

class BoundingBox2D
{
public:
    BoundingBox2D();
    BoundingBox2D(const float minX, const float minY, const float maxX, const float maxY);
    BoundingBox2D(const glm::vec2& min, const glm::vec2& max);
    ~BoundingBox2D();


    void setMin(const glm::vec2& min);
    void setMax(const glm::vec2& max);

    glm::vec2 Min() const;
    glm::vec2 Max() const;
    glm::vec2 Size() const;

private:
    bool isValid() const;

private:
    glm::vec2 mMin;
    glm::vec2 mMax;
};

#endif //BOUNDINGBOX_HPP
