//
// Created by George on 10/03/2019.
//

#include "main.h"
#include "../model/Robots/Robots.h"
#include "../model/Castle/Castle.h"
#include "../model/Cannon/Cannon.h"
#include "../model/SpaceShip/Spaceship.h"
#include "../model/Skybox/Skybox.h"
#include "../model/Pottery/Pottery.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <GL/freeglut.h>

#define ROBOT_SCALE 0.4

static float angle = 0.0;  //Rotation angle for viewing
static double eyeX = 0, eyeY = 0, eyeZ = 0, lookX = 0, lookY = 0, lookZ = -1;
static bool spaceView = false;
static double minBoundary = -950, maxBoundary = 950;

Castle* castle;
Skybox* skybox;
Spaceship* spaceship;

void drawFloor()
{
    float white[4] = {1., 1., 1., 1.};
    float black[4] = {0};
    glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);

    //glMaterialfv(GL_FRONT, GL_SPECULAR, black);

    //The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
    glBegin(GL_QUADS);
    for(int i = -500; i < 500; i++)
    {
        for(int j = -500;  j < 500; j++)
        {
            glVertex3f(i, -10, j);
            glVertex3f(i, -10, j+1);
            glVertex3f(i+1, -10, j+1);
            glVertex3f(i+1, -10, j);
        }
    }
    glEnd();
    //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}


void display()
{
    float lpos[4] = {100., 100., 100., 1.0};  //light's position

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (spaceView) {
        gluLookAt(0, spaceship->animValues.y + 5, -200, 0,
                -spaceship->animValues.y, 0, 0, 0, 1);
    } else {
        gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 1, 0);
    }

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    
    glColor3f(1, 0, 0);
    glPushMatrix();
        glTranslated(-0.5*castle->getLength(), castle->getHeight(), 0.5*castle->getLength()+5);
        glRotatef(90, 0, 1, 0);
        glScalef(ROBOT_SCALE, ROBOT_SCALE, ROBOT_SCALE);
        glPushMatrix();
            glRotatef(-90, 0, -1, 0);
            float dir[] = {-1, -1, 0};
            float spotlight[] = {-robots[0]->deltaZ-10, robots[0]->deltaY, robots[0]->deltaX+10, 1.0f};
            // glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
            //glLightfv(GL_LIGHT1, GL_POSITION, spotlight);
        glPopMatrix();
        if (!robots[0]->dead) {
            robots[0]->drawRobot();
        }
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.5*castle->getLength(), castle->getHeight(), 0.5*castle->getLength()+5);
        glRotatef(180, 0, 1, 0);
        glScalef(ROBOT_SCALE, ROBOT_SCALE, ROBOT_SCALE);
        if (!robots[1]->dead) {
            robots[1]->drawRobot();
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, -200);
        castle->drawCastle();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,  0, -150);
        glScalef(2, 2, 2);
        cannons[0]->drawCannon();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, -200);
        spaceship->drawSpaceship();
    glPopMatrix();

    glPushMatrix();
        skybox->drawSkybox();
    glPopMatrix();


    glPushMatrix();
        drawFloor();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(17, 10, 250);
        glColor4f(0.8, 0.93, 1, 0.25);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawWineGlass(true);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-17, 10, 250);
        glColor4f(0.8, 0.93, 1, 1);
        drawWineGlass(false);
    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}


/**
 * The keyboard function event
 * @param key Key pressed
 * @param x Mouse X position
 * @param y Mouse Y position
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'p':
            if (!robots[0]->moving) {
                glutTimerFunc(100, patrolAnim, 0);

            }
            if (!robots[1]->moving) {
                glutTimerFunc(100, patrolAnim, 1);
            }
            break;
        case 'd':
            // Remove 1 robot at a time
            if (!robots[0]->dying && !robots[0]->dead) {
                glutTimerFunc(100, dieAnim, 0);
            }
            else if(!robots[1]->dying && !robots[0]->dead) {
                glutTimerFunc(100, dieAnim, 1);
            }
            break;
        case 'c':
            if (!cannons[0]->firing) {
                glutTimerFunc(100, fireCannonAnim, 0);
            }
            break;
        case 'u':
            if (castle->gate.closed && !castle->gate.opening) {
                glutTimerFunc(100, castle->openGateAnim, castle->gate.angle);
            } else if (castle->gate.open && !castle->gate.closing) {
                glutTimerFunc(100, castle->closeGateAnim, castle->gate.angle);
            }
            break;
        case 's':
            if (spaceship->isGrounded()) {
                glutTimerFunc(100, spaceship->takeOffAnim, spaceship->animValues.takeOffValue);
            }
    }

    glutPostRedisplay();
}


//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    double radAngle = angle * M_PI / 180;

    if (key == GLUT_KEY_LEFT) {
        angle-=5;
        radAngle = angle * M_PI/180;
    }
    else if (key == GLUT_KEY_RIGHT) {
        angle+=5;
        radAngle = angle * M_PI / 180;
    }
    else if (key == GLUT_KEY_UP && (glutGetModifiers() == GLUT_ACTIVE_SHIFT)) {
        eyeY += 1 * cos(radAngle);
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        eyeY -= 1 * cos(radAngle);
    }
    else if (key == GLUT_KEY_UP && (eyeZ > minBoundary || (eyeX < maxBoundary && eyeX > minBoundary))) {
        eyeX += 100 * sin(radAngle);
        eyeZ -= 100 * cos(radAngle);
    }
    else if (key == GLUT_KEY_DOWN && (eyeZ < maxBoundary || (eyeX < maxBoundary && eyeX > minBoundary))) {
        eyeX -= 100 * sin(radAngle);
        eyeZ += 100 * cos(radAngle);
    }
    else if (key == GLUT_KEY_HOME) {
        spaceView ^= true;
    }

    if (eyeY <= 20.5) {
        eyeY = 20.5;
    }

    if (eyeZ >= maxBoundary) {
        eyeZ = maxBoundary;
    } else if (eyeZ <= minBoundary) {
        eyeZ = minBoundary;
    }

    if (eyeX >= maxBoundary) {
        eyeX = maxBoundary;
    } else if (eyeX <= minBoundary) {
        eyeX = minBoundary;
    }

    lookX = eyeX + 100*sin(radAngle);
    lookY = eyeY;
    lookZ = eyeZ - 100*cos(radAngle);



    glutPostRedisplay();
}


void initObjects() {
    skybox = new Skybox();
    skybox->loadTextures();

    //castle = new Castle(300, 100);
    //castle = new Castle(450, 200);
    castle = new Castle(900, 300);
    castle->loadTex();

    for(int i=0; i < 4;i++) {
        castle->walls[i]->color[0] = 1.0f;
        castle->walls[i]->color[1] = 0.65f;
        castle->walls[i]->color[2] = 0.0f;
        castle->walls[i]->color[3] = 1.0f;
    }


    for (int i=0; i < 2; i++) {
        robots[i] = new Robots();
        robots[i]->patrolDistance = 0.8*castle->getLength();
    }

    for (int i = 0; i < 4; i++) {
        cannons[i] = new Cannon();
        cannons[i]->loadMeshFile("../assets/Cannon.off");
        cannons[i]->cannonX = 0;
        cannons[i]->cannonY = 10;
        cannons[i]->cannonZ = cannons[i]->getLength() + castle->getLength() / 4;
        cannons[i]->tilt = 30;
        cannons[i]->setCannonBall();
    }

    spaceship = new Spaceship(30);
    spaceship->loadTex();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

    glEnable(GL_LIGHTING);					//Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float grey[4] = {0.2, 0.2, 0.2, 0.2};
    float white[4]  = {0.5, 0.5, 0.5, 0.5};
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 5, 5000);
    //gluPerspective(80.0, 1.0, 100.0, 5000.0);   //Perspective projection from Skybox lab

    initObjects();

    eyeZ = castle->getLength();
    eyeY = 5.5;
}


//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (1920, 1080);//(600, 600);

    glutInitWindowPosition (1, 1);//(10, 10);
    glutCreateWindow ("Main");
    initialize();

    //glutFullScreen();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    display();

    glutMainLoop();
    return 0;
}
