//
// Created by George on 1/04/2019.
//

#include "Spaceship.h"
#include "../Shapes/Shapes.h"
#include "../../Controller/loadTGA.h"

Spaceship::AnimValues Spaceship::animValues;

Spaceship::Spaceship(int radius){
    this->radius = radius;
    animValues.takeOffValue = 0;
    animValues.idleValue = 0;
    animValues.dieValue = 0;
    animValues.grounded = true;

    animValues.x = 0;
    animValues.y = 0;
    animValues.z = 0;
}


void Spaceship::loadTex() {
    glGenTextures(1, &wingTex); 				// Create a Texture object
    glBindTexture(GL_TEXTURE_2D, wingTex);		//Use this texture
    loadTGA("../assets/metal.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


int Spaceship::getHealth() {
    return this->health;
}


void Spaceship::setRadius(int radius) {
    this->radius = radius;
}


int Spaceship::getRadius() {
    return this->radius;
}


bool Spaceship::isGrounded() {
    return this->animValues.grounded;
}


/**
 * Takes a default damage amount (5)
 */
void Spaceship::takeDamage() {
    this->health -= 5;
    checkAlive();
}


/**
 * Take damage given a specified amount
 * @param damage
 */
void Spaceship::takeDamage(int damage) {
    this->health -= damage;
    checkAlive();
}

/**
 * Checks if a apaceship is alive,
 * if not start destroy animation
 */
void Spaceship::checkAlive() {
    if (this->health <= 0) destroyAnim(1);
}


void Spaceship::drawWingTips() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, wingTex);
    glBegin(GL_TRIANGLES);
        // Right wing tip
        glTexCoord2f(1,  0);    glVertex3f(radius, 10, 0);
        glTexCoord2f(1, 1);     glVertex3f(radius, 30, 0);
        glTexCoord2f(0, 1);     glVertex3f(radius+20, 10, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
        // Left wing tip
        glTexCoord2f(1,  0);    glVertex3f(-radius, 10, 0);
        glTexCoord2f(1, 1);     glVertex3f(-radius, 30, 0);
        glTexCoord2f(0, 1);     glVertex3f(-(radius+20), 10, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
        // Front wing tip
        glTexCoord2f(1, 0);    glVertex3f(0, 10,  radius);
        glTexCoord2f(1, 1);    glVertex3f(0, 30,  radius);
        glTexCoord2f(0, 1);    glVertex3f(0, 10, radius+20);
    glEnd();

    glBegin(GL_TRIANGLES);
        //Rear wing tip
        glTexCoord2f(1, 0);     glVertex3f(0, 10, -radius);
        glTexCoord2f(1, 1);     glVertex3f(0, 30, -radius);
        glTexCoord2f(0, 1);     glVertex3f(0, 10, -(radius+20));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between right and front (SE)
        glTexCoord2f(1, 0);     glVertex3f(radius*cos(45*M_PI/180), 10, radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(radius*cos(45*M_PI/180), 30, radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f((radius+20)*cos(45*M_PI/180), 10, (radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between left and front (SW)
        glTexCoord2f(1, 0);     glVertex3f(-radius*cos(45*M_PI/180), 10, radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(-radius*cos(45*M_PI/180), 30, radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f(-(radius+20)*cos(45*M_PI/180), 10, (radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between right and back (NE)
        glTexCoord2f(1, 0);     glVertex3f(radius*cos(45*M_PI/180), 10, -radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(radius*cos(45*M_PI/180), 30, -radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f((radius+20)*cos(45*M_PI/180), 10, -(radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between left and back (NW)
        glTexCoord2f(1, 0);     glVertex3f(-radius*cos(45*M_PI/180), 10, -radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(-radius*cos(45*M_PI/180), 30, -radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f(-(radius+20)*cos(45*M_PI/180), 10, -(radius+20)*sin(45*M_PI/180));
    glEnd();

    glTexCoord2f(0, 0);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}


void Spaceship::drawSpaceship() {

    //Body
    glPushMatrix();
        glTranslatef(0, 10, 0);
        glColor4f(0.96, 0.79, 0.65, 1);
        drawCylinder(radius, 3*radius);
    glPopMatrix();

    // Top part
    glPushMatrix();
        glTranslatef(0, radius*3+10, 0);
        glColor4f(0.96, 0.79, 0.65, 1);
        drawCone(radius, radius);
    glPopMatrix();

    //Wing tips
    glPushMatrix();
        drawWingTips();
    glPopMatrix();

    //Boosters
    float xDir = 1;
    float zDir = 1;
    for(int i=0; i < 2; i++) {
        if (i % 2 == 1){
            xDir = -1;
            zDir = -1;
        }
        glPushMatrix();
            glTranslatef(xDir * 0.4*radius, 0, zDir * 0.4*radius);
            drawCylinder(0.3*radius, 20);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(xDir * 0.4*radius, 0, -zDir * 0.4*radius);
            drawCylinder(0.3*radius, 20);
        glPopMatrix();
    }
}


void Spaceship::destroyAnim(int index) {
    return;
}


void Spaceship::takeOffAnim(int value) {
    if (value < TAKE_OFF_TIME) {
        animValues.grounded = false;
        animValues.y += 10;
        animValues.takeOffValue ++;//* value = * value + 1;
        glutPostRedisplay();
        glutTimerFunc(100, takeOffAnim, animValues.takeOffValue);
    } else {
        animValues.takeOffValue = 0;
        glutPostRedisplay();
    }
}