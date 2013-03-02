#include "texture.hpp"

#include "color.hpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glfw.h>

Texture2D::Texture2D()
: mData(NULL)
, mHeight(0)
, mWidth(0)
{}

void Texture2D::setTexture(Color::rgb * data, uint height, uint width)
{
    assert(NULL == mData);
    mData = data;
    mHeight = height;
    mWidth = width;
}

Texture2D::~Texture2D()
{
    if(mData)
        delete[] mData;
}

char const * const Texture2D::getData() const
{
    return &(mData[0].r);
}

uint Texture2D::getHeight() const
{
    return mHeight;
}

uint Texture2D::getWidth() const
{
    return mWidth;
}

void Texture2D::loadBMP_custom(const char * imagepath, Texture2D & texture)
{
    assert(NULL == texture.mData);
    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    uint dataPos;
    uint imageSize;

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
    texture.mWidth  = *(int*)&(header[0x12]);
    texture.mHeight = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=texture.mWidth*texture.mHeight;
    if (dataPos==0)      dataPos=headerSize; // The BMP header is done that way

    // Create a buffer
    texture.mData = new Color::rgb[imageSize];

    fread(texture.mData, 1, imageSize, file);

    fclose (file);
}
