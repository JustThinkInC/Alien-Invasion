//
// Created by George on 7/04/2019.
//

#ifndef ASSIGNMENT1_SKYBOX_H
#define ASSIGNMENT1_SKYBOX_H
#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures
#include <GL/freeglut.h>

class Skybox {
private:
    GLuint texId[6];
public:
    void loadTextures();
    void drawSkybox();

    Skybox() {};
};


#endif //ASSIGNMENT1_SKYBOX_H
