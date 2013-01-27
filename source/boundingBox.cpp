#include "boundingBox.hpp"

using namespace glm;

BoundingBox2D::BoundingBox2D()
: mMin(vec2(0))
, mMax(vec2(0))
{
    assert( isValid() );
}

BoundingBox2D::BoundingBox2D(const float minX, const float minY, const float maxX, const float maxY)
: mMin(vec2(minX, minY))
, mMax(vec2(maxX, maxY))
{
    assert( isValid() );
}

BoundingBox2D::BoundingBox2D(const glm::vec2& min, const glm::vec2& max)
: mMin(min)
, mMax(max)
{
    assert( isValid() );
}

BoundingBox2D::~BoundingBox2D()
{

}

void BoundingBox2D::setMin(const glm::vec2& min)
{
    mMin = min;
    assert( isValid() );
}

void BoundingBox2D::setMax(const glm::vec2& max)
{
    mMax = max;
    assert( isValid() );
}

glm::vec2 BoundingBox2D::Min() const
{
    return mMin;
}

glm::vec2 BoundingBox2D::Max() const
{
    return mMax;
}

glm::vec2 BoundingBox2D::Size() const
{
    return mMax - mMin;
}

bool BoundingBox2D::isValid() const
{
    return ( mMin.x <= mMax.x && mMin.y <= mMax.y );
}
