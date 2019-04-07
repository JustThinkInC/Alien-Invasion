#include "Castle.h"
#include "../Shapes/Shapes.h"
#include "../../controller/loadTGA.h"
#include <math.h>
#include <GL/freeglut.h>
using namespace std;

Castle::Wall* Castle::walls[5];
Castle::Gate Castle::gate;

/**
 * Create a castle of given side-length
 * and given wall height
 * @param length
 * @param height
 */
Castle::Castle(int length, int height){
    this->length = length;
    this->height = height;
    for(int i=0;i<5;i++) {
        walls[i] = new Wall;
        walls[i]->length = this->length;
        walls[i]->number = i;
    }
    xFront = 5;
    yFront = yLevel;
    zFront = 0.5 * length;
}


void Castle::loadTex() {
    glGenTextures(1, &doorTexId); 				// Create a Texture object
    glBindTexture(GL_TEXTURE_2D, doorTexId);		//Use this texture
    loadTGA("../assets/gate.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void Castle::drawGate(float length) {
    float halfLength = length * 0.5;
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, doorTexId);
    glBegin(GL_QUADS);
        glVertex3f(-halfLength, halfLength, length);    glTexCoord2f(1, 0);
        glVertex3f(halfLength, halfLength, length);     glTexCoord2f(1, 1);
        glVertex3f(halfLength, -halfLength, length);    glTexCoord2f(0, 1);
        glVertex3f(-halfLength, -halfLength, length);   glTexCoord2f(0, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}


int Castle::getColumns() {
    return this->columns;
}


void Castle::drawCastle() {
    // Front wall
    glPushMatrix();
        glTranslatef(5, yLevel, 0.5*length);
        glColor4f(0, 1, 0, 1);// glColor4f(walls[0]->color[0], walls[0]->color[1], walls[0]->color[2], walls[0]->color[3]);
        glScalef(1.05, ((float) height)/length, 10.0/length);
        drawCube(length);
    glPopMatrix();

    // Right wall
    glPushMatrix();
        glTranslatef(0.5*length, yLevel, 0);
        glColor4f(walls[1]->color[0], walls[1]->color[1], walls[1]->color[2], walls[1]->color[3]);
        glRotatef(90, 0, 1, 0);
        glScalef(1, ((float) height)/length, 10.0 / length);
        drawCube(length);
    glPopMatrix();

    // Back wall
    glPushMatrix();
        glTranslatef(0, yLevel, -0.5*length-10);
       glColor4f(1, 0, 0, 1);// glColor4f(walls[2]->color[0], walls[2]->color[1], walls[2]->color[2], walls[2]->color[3]);
        glScalef(1, ((float) height)/length, 10.0/length);
        drawCube(length);
    glPopMatrix();

    // Left wall
    glPushMatrix();
        glTranslatef(-0.5*length, yLevel, 0);
        glColor4f(walls[3]->color[0], walls[3]->color[1], walls[3]->color[2], walls[3]->color[3]);
        glRotatef(90, 0, 1, 0);
        glScalef(1, ((float) height)/length, 10.0/length);
        drawCube(length);
    glPopMatrix();


    // Gate
    glPushMatrix();
        glTranslatef(xFront/2, 12, 0.5 * length + 7);
        glRotatef(gate.angle, 1, 0, 0);
        glTranslatef(xFront/2, 12, (0.5 * length + 7)/10);//12);
        glColor4f(gate.color[0], gate.color[1], gate.color[2], gate.color[3]);
        glScalef(0.75, 1, 5 / length);
        drawGate(height);
    glPopMatrix();

}


/**
 * Destroy a wall and play the fade animation
 * @param wall
 */
void Castle::destroyWall(Castle::Wall* wall) {
    wall->health = 0;
    glutTimerFunc(0, fadeWallAnim, wall->number);
}


/**
 * Fade a wall, this sequence should play after
 * a wall has been destroyed
 * @param wallNum
 */
void Castle::fadeWallAnim(int wallNum) {
    Wall* wall = walls[wallNum];
    wall->color[3] -= 0.1;
    if (wall->color[3] < 0) {
        wall->color[3] = 0;
        wall->destroyed = true;
        walls[wallNum] = NULL;
        return;
    }
    glutPostRedisplay();
    glutTimerFunc(50, fadeWallAnim, wallNum);
}


/**
 * Animation for opening the castle gate
 * @param angle
 */
void Castle::openGateAnim(int angle) {
    if (gate.angle < 90 && !gate.open) {
        gate.angle += 5;
        gate.closed = false;
        gate.closing = false;
        gate.opening = true;
        glutPostRedisplay();
        glutTimerFunc(100, openGateAnim, gate.angle);
    } else {
        gate.opening = false;
        gate.open = true;
    }
}


/**
 * Animation for closing the gate
 * @param angle
 */
void Castle::closeGateAnim(int angle) {
    if (gate.angle > 0 && !gate.closed) {
        gate.angle -= 5;
        gate.open = false;
        gate.opening = false;
        gate.closing = true;
        glutPostRedisplay();
        glutTimerFunc(100, closeGateAnim, gate.angle);
    } else {
        gate.closed = true;
        gate.closing = false;
    }
}


/**
 * Damage a castle wall
 * @param damage
 * @param wall
 */
void Castle::damageWall(int damage, Castle::Wall *wall) {
    wall->health -= damage;
    if (wall->health <= 0) destroyWall(wall);
}


void Castle::setLength(int length) {
    this->length = length;
}


int Castle::getLength() {
    return this->length;
}

int Castle::getHeight() {
    return this->height;
}