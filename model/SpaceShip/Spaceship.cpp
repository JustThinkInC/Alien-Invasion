//
// Created by George on 1/04/2019.
//

#include "Spaceship.h"
#include "../Shapes/Shapes.h"

Spaceship* spaceships[1];

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
    return this->grounded;
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

void Spaceship::drawSpaceship() {

    // Hat for spaceship...
    glPushMatrix();
        glTranslatef(x, y+0.5*getRadius(), z);
        glColor4f(1, 0, 0, 1);
        glRotatef(-180, 0, 1, 0);
        drawCone(radius, radius);
    glPopMatrix();

    // Inside
    glPushMatrix();
        glTranslatef(x, y, z);
        glColor4f(0.65, 0.8, 0.84, 0.5);
        drawSphere(radius);
    glPopMatrix();

    //TODO: FIX THIS
    // Outer ring
    glPushMatrix();
        glTranslatef(x, y, z);
        glColor4f(0, 1, 0, 1);
        glRotatef(90, 1, 0, 0);
       glutSolidTorus(5, 20, 100, 100);
    glPopMatrix();
}


void destroyAnim(int index) {
    return;
}


void takeOffAnim(int index) {
    Spaceship* spaceship = spaceships[index];

    int* value = &spaceship->animValues.takeOffValue;
    if (*value < TAKE_OFF_TIME) {
        spaceship->y += 10;
        * value = * value + 1;
        glutPostRedisplay();
        glutTimerFunc(100, takeOffAnim, index);
    } else {
        * value = 0;
        glutPostRedisplay();
    }
}