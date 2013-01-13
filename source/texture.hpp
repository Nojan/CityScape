#ifndef TEXTURE_HPP
#define TEXTURE_HPP

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

    void setTexture(Color::rgb * data, unsigned int height, unsigned int width);

    char const * const getData() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;

private:
    Color::rgb * mData;
    unsigned int mHeight;
    unsigned int mWidth;
};


#endif
