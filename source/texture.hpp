#ifndef TEXTURE_HPP
#define TEXTURE_HPP

class Texture2D
{
    struct rgb
    {
        char r;
        char g;
        char b;
    };

public:
    Texture2D();
    ~Texture2D();

    static void loadBMP_custom(const char * imagepath, Texture2D & texture);

    char const * const getData() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;

private:
    rgb * data;
    unsigned int height;
    unsigned int width;
};


#endif
