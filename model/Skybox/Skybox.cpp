//
// Created by George on 7/04/2019.
//

#include <GL/gl.h>
#include "Skybox.h"
#include "../../controller/loadTGA.h"

void Skybox::loadTextures()				// Load bitmaps And Convert To Textures
{glGenTextures(6, texId); 		// Create texture ids
    // *** left ***
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("../assets/Skybox/rt.tga");
    //loadTGA("../assets/left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("../assets/Skybox/ft.tga");
    //loadTGA("../assets/front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("../assets/Skybox/lf.tga");
    //loadTGA("../assets/right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** back***
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("../assets/Skybox/bk.tga");
    //loadTGA("../assets/back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("../assets/Skybox/up.tga");
    //loadTGA("../assets/top.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}



void Skybox::drawSkybox()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    ////////////////////// LEFT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    glBegin(GL_QUADS);
    glVertex3f(-1000,  0, 1000);  glTexCoord2f(1, 0);
    glVertex3f(-1000, 0., -1000); glTexCoord2f(1, 1);
    glVertex3f(-1000, 1000., -1000);  glTexCoord2f(0, 1);
    glVertex3f(-1000, 1000, 1000);    glTexCoord2f(0, 0);
    glEnd();

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    glBegin(GL_QUADS);
    glVertex3f(-1000,  0, -1000); glTexCoord2f(1, 0);
    glVertex3f(1000, 0., -1000); glTexCoord2f(1, 1);
    glVertex3f(1000, 1000, -1000);    glTexCoord2f(0, 1);
    glVertex3f(-1000,  1000, -1000);  glTexCoord2f(0, 0);
    glEnd();


    ////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    glBegin(GL_QUADS);
    glVertex3f(1000,  0, -1000);  glTexCoord2f(1, 0);
    glVertex3f(1000, 0, 1000);    glTexCoord2f(1, 1);
    glVertex3f(1000, 1000,  1000);    glTexCoord2f(0, 1);
    glVertex3f(1000,  1000,  -1000);  glTexCoord2f(0, 0);
    glEnd();


    ////////////////////// REAR WALL ////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    glBegin(GL_QUADS);
    glVertex3f( 1000, 0, 1000); glTexCoord2f(1, 0);
    glVertex3f(-1000, 0,  1000);glTexCoord2f(1, 1);
    glVertex3f(-1000, 1000,  1000);glTexCoord2f(0, 1);
    glVertex3f( 1000, 1000, 1000);glTexCoord2f(0, 0);
    glEnd();

    /////////////////////// TOP //////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    glBegin(GL_QUADS);
    glVertex3f(-1000, 1000, -1000);glTexCoord2f(1, 0);
    glVertex3f(1000, 1000,  -1000);glTexCoord2f(1, 1);
    glVertex3f(1000, 1000,  1000);glTexCoord2f(0, 1);
    glVertex3f(-1000, 1000, 1000); glTexCoord2f(0, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}