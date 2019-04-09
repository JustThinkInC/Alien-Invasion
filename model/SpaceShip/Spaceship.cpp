//
// Created by George on 1/04/2019.
//

#include "Spaceship.h"
#include "../Shapes/Shapes.h"
#include "../../Controller/loadTGA.h"

Spaceship::AnimValues Spaceship::animValues;

Spaceship::Spaceship(int radius) {
    this->radius = radius;
    this->boosterRadius = 0.4 * radius;
    animValues.takeOffValue = 0;
    animValues.idleValue = 0;
    animValues.dieValue = 0;
    animValues.grounded = true;

    animValues.x = 0;
    animValues.y = 0;
    animValues.z = 0;
}


/**
 * Load the textures to use on spaceship
 */
void Spaceship::loadTex() {
    glGenTextures(2, texIds);

    //Wing tip texture
    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    loadTGA("../assets/metal.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Flames texture
    glBindTexture(GL_TEXTURE_2D, texIds[1]);
    loadTGA("../assets/explosion.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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


/**
 * Draw a single flame particle and texture it for booster
 */
void Spaceship::drawFlameParticle() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texIds[1]);
    glBegin(GL_QUADS);
            glVertex3f(0, 0, 0);        glTexCoord2f(1, 0);
            glVertex3f(1, 0, 0);        glTexCoord2f(1, 1);
            glVertex3f(1, 1, 0);        glTexCoord2f(0, 1);
            glVertex3f(0, 1, 0);        glTexCoord2f(0, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}


void Spaceship::drawBoosters() {
    //Boosters
    float xDir = 1;
    float zDir = 1;
    for(int i=0; i < 2; i++) {
        if (i % 2 == 1) {
            xDir = -1;
            zDir = -1;
        }

        glPushMatrix();
            glTranslatef(xDir * boosterRadius, animValues.y, zDir * boosterRadius);
            drawCylinder(0.3 * radius, 20);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(xDir * boosterRadius, animValues.y, -zDir * boosterRadius);
            drawCylinder(0.3 * radius, 20);
        glPopMatrix();
    }
}


/**
 * Draw a flame per booster
 * Each flame has 50 particles
 */
void Spaceship::drawFlames() {
    if (animValues.grounded) return;
    float xDir = 1;
    float zDir = 1;
    // Create a flame per booster
    for (int i = 0; i < 4; i++) {
        // Decide whether to switch directions
        if (i % 2 == 1) {
            xDir = -1;
            zDir = -1;
        }

        // Creat the 50 particles per booster
        for (int j = 0; j < FLAMES; j++) {
            int roundedRadius = round(boosterRadius);
            int maxY = round(animValues.y) - 10;
            int minY = round(animValues.y) - 40;

            double flameA = rand() * 2 * M_PI;  // Random angle
            double flameR = (roundedRadius * 0.75) * sqrt(rand() % roundedRadius); // Random radius for flame

            double flameX = flameR * cos(flameA); // X value based on randomised angle and radius
            double flameZ = rand() % roundedRadius; // Random z value between 0 and radius
            double flameY = rand() % (maxY - minY + 1) + minY;

            // Draw the flame in the +Z direction
            glPushMatrix();
                glTranslatef(xDir * (flameX - 5), flameY, zDir * flameZ);
                glScalef(10, 10, 10);
                drawFlameParticle();
            glPopMatrix();

            // Draw the flame in the -Z direction
            glPushMatrix();
                glTranslatef(xDir * (flameX - 5), flameY, -zDir * flameZ);
                glScalef(5, 5, 5);
                drawFlameParticle();
            glPopMatrix();
        }
    }
}


/**
 * Draw the wing tips and texture them
 */
void Spaceship::drawWingTips() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    glBegin(GL_TRIANGLES);
        // Right wing tip
        glTexCoord2f(1,  0);    glVertex3f(radius, 10+animValues.y, 0);
        glTexCoord2f(1, 1);     glVertex3f(radius, 30+animValues.y, 0);
        glTexCoord2f(0, 1);     glVertex3f(radius+20, 10+animValues.y, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
        // Left wing tip
        glTexCoord2f(1,  0);    glVertex3f(-radius, 10+animValues.y, 0);
        glTexCoord2f(1, 1);     glVertex3f(-radius, 30+animValues.y, 0);
        glTexCoord2f(0, 1);     glVertex3f(-(radius+20), 10+animValues.y, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
        // Front wing tip
        glTexCoord2f(1, 0);    glVertex3f(0, 10+animValues.y,  radius);
        glTexCoord2f(1, 1);    glVertex3f(0, 30+animValues.y,  radius);
        glTexCoord2f(0, 1);    glVertex3f(0, 10+animValues.y, radius+20);
    glEnd();

    glBegin(GL_TRIANGLES);
        //Rear wing tip
        glTexCoord2f(1, 0);     glVertex3f(0, 10+animValues.y, -radius);
        glTexCoord2f(1, 1);     glVertex3f(0, 30+animValues.y, -radius);
        glTexCoord2f(0, 1);     glVertex3f(0, 10+animValues.y, -(radius+20));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between right and front (SE)
        glTexCoord2f(1, 0);     glVertex3f(radius*cos(45*M_PI/180), 10+animValues.y, radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(radius*cos(45*M_PI/180), 30+animValues.y, radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f((radius+20)*cos(45*M_PI/180), 10+animValues.y, (radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between left and front (SW)
        glTexCoord2f(1, 0);     glVertex3f(-radius*cos(45*M_PI/180), 10+animValues.y, radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(-radius*cos(45*M_PI/180), 30+animValues.y, radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f(-(radius+20)*cos(45*M_PI/180), 10+animValues.y, (radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between right and back (NE)
        glTexCoord2f(1, 0);     glVertex3f(radius*cos(45*M_PI/180), 10+animValues.y, -radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(radius*cos(45*M_PI/180), 30+animValues.y, -radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f((radius+20)*cos(45*M_PI/180), 10+animValues.y, -(radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between left and back (NW)
        glTexCoord2f(1, 0);     glVertex3f(-radius*cos(45*M_PI/180), 10+animValues.y, -radius*sin(45*M_PI/180));
        glTexCoord2f(1, 1);     glVertex3f(-radius*cos(45*M_PI/180), 30+animValues.y, -radius*sin(45*M_PI/180));
        glTexCoord2f(0, 1);     glVertex3f(-(radius+20)*cos(45*M_PI/180), 10+animValues.y, -(radius+20)*sin(45*M_PI/180));
    glEnd();

    glTexCoord2f(0, 0);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}


/**
 * Draw the entire spaceship
 */
void Spaceship::drawSpaceship() {
    //Body
    glPushMatrix();
        glTranslatef(0, 10+animValues.y, 0);
        glColor4f(0.96, 0.79, 0.65, 1);
        drawCylinder(radius, 3*radius);
    glPopMatrix();

    // Top part
    glPushMatrix();
        glTranslatef(0, radius*3+10+animValues.y, 0);
        glColor4f(0.96, 0.79, 0.65, 1);
        drawCone(radius, radius);
    glPopMatrix();

    //Wing tips
    glPushMatrix();
        drawWingTips();
    glPopMatrix();

    // Boosters
    glPushMatrix();
        drawBoosters();
    glPopMatrix();

    // Render the flames
    glPushMatrix();
        drawFlames();
    glPopMatrix();
}


/**
 * Animation for destroying spaceship
 * @param index
 */
void Spaceship::destroyAnim(int index) {
    return;
}


/**
 * Animation for spaceship takeoff
 * @param value
 */
void Spaceship::takeOffAnim(int value) {
    if (value < TAKE_OFF_TIME) {
        animValues.grounded = false;
        animValues.y += 10;
        animValues.takeOffValue ++;
        glutPostRedisplay();
        glutTimerFunc(100, takeOffAnim, animValues.takeOffValue);
    }
}