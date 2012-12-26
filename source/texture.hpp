#ifndef TEXTURE_HPP
#define TEXTURE_HPP

class Texture2D
{
public:
    Texture2D();
    ~Texture2D();

    static void loadBMP_custom(const char * imagepath, Texture2D & texture);

    char const * const getData() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;

private:
    char * data;
    unsigned int height;
    unsigned int width;
};

//GLuint loadTGA_glfw(const char * imagepath); //need MAJ
//GLuint loadDDS(const char * imagepath); //need MAj

#endif
