#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "types.hpp"

namespace Color
{
    struct rgb;
}

class Texture2D
{
public:
    Texture2D();
    ~Texture2D();

    static void loadBMP_custom(const char * imagepath, Texture2D & texture);

    void setTexture(Color::rgb * data, uint height, uint width);

    char const * const getData() const;
    uint getHeight() const;
    uint getWidth() const;

private:
    Color::rgb * mData;
    uint mHeight;
    uint mWidth;
};


#endif
