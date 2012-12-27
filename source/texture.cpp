#include "texture.hpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glfw.h>

Texture2D::Texture2D()
: data(NULL)
, height(0)
, width(0)
{}

Texture2D::~Texture2D()
{
    delete[] data;
}

char const * const Texture2D::getData() const
{
    return &(data[0].r);
}

unsigned int Texture2D::getHeight() const
{
    return height;
}

unsigned int Texture2D::getWidth() const
{
    return width;
}

void Texture2D::loadBMP_custom(const char * imagepath, Texture2D & texture)
{
    assert(NULL == texture.data);
    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    assert(NULL != file);

    // Read the header, i.e. the 54 first bytes
    const size_t headerSize = 54;

    // If less than 54 byes are read, problem
    if( fread(header, 1, headerSize, file) != headerSize )
        assert(false);
    // A BMP files always begins with "BM"
    assert( header[0]=='B' && header[1]=='M' );

    // Make sure this is a 24bpp file
    assert( *(int*)&(header[0x1E]) == 0 );
    assert( *(int*)&(header[0x1C]) ==24 );

    // Read the information about the image
    dataPos        = *(int*)&(header[0x0A]);
    imageSize      = *(int*)&(header[0x22]);
    texture.width  = *(int*)&(header[0x12]);
    texture.height = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=texture.width*texture.height;
    if (dataPos==0)      dataPos=headerSize; // The BMP header is done that way

    // Create a buffer
    texture.data = new rgb [imageSize];

    fread(texture.data, 1, imageSize, file);

    fclose (file);
}
