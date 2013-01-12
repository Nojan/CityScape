#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GL/glew.h>

class Texture2D;

class Skybox {
public:
    static Skybox* CreateSkyboxFrom(const char * xPosPath, const char * xNegPath,
                                    const char * yPosPath, const char * yNegPath,
                                    const char * zPosPath, const char * zNegPath);

    Skybox(Texture2D& xPos, Texture2D& xNeg, Texture2D& yPos, Texture2D& yNeg, Texture2D& zPos, Texture2D& zNeg);
    ~Skybox();

    void Draw(GLuint programID);

private:
    GLuint mIndexBufferId;
    GLuint mVertexBufferId;
    GLuint uvbuffer;
    GLuint mTextureBufferId;
};

#endif
