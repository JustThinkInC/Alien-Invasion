//
// Created by George on 10/03/2019.
//

#include "main.h"
#include "../model/Robots/Robots.h"
#include "../model/Castle/Castle.h"
#include "../model/Cannon/Cannon.h"
#include "../model/SpaceShip/Spaceship.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
#include <vector>

#define ROBOT_SCALE 0.1

float angle = 0.0;  //Rotation angle for viewing
float cam_hgt = 10;
float cam_dist = 200;

//std::vector<Robots> robots;
//Robots* robots[10];
//Robots* robots[2] = {new Robots(), new Robots()};
Castle* castle = new Castle(150, 50);

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
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glVertex3f(i+1, 0, j+1);
            glVertex3f(i+1, 0, j);
        }
    }
    glEnd();
    //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
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
            if (!robots[0]->dying && !robots[0]->dead) {
                glutTimerFunc(100, dieAnim, 0);
            }
//            if(!robots[1]->dying && !robots[0]->dead) {
//                glutTimerFunc(100, dieAnim, 1);
//            }
            break;
        case 'c':
            if (!cannons[0]->firing) {
                glutTimerFunc(100, fireCannonAnim, 0);
            } else {
                cout << "OH OH" << endl;
            }
            break;

        case 'u':
            if (castle->gate.closed && !castle->gate.opening) {
                glutTimerFunc(100, castle->openGateAnim, castle->gate.angle);
            } else if (castle->gate.open && !castle->gate.closing) {
                glutTimerFunc(100, castle->closeGateAnim, castle->gate.angle);
            }
    }
    glutPostRedisplay();
}


//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle-=5;
    else if(key == GLUT_KEY_RIGHT) angle+=5;
    else if(key == GLUT_KEY_UP && (glutGetModifiers() == GLUT_ACTIVE_SHIFT)) cam_hgt++;
    else if(key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT) cam_hgt--;
    else if(key == GLUT_KEY_UP) cam_dist--;
    else if(key == GLUT_KEY_DOWN) cam_dist++;

    if(cam_hgt > 200) cam_hgt = 200;
    else if(cam_hgt < 10) cam_hgt = 10;

    glutPostRedisplay();
}


void display()
{
    float lpos[4] = {100., 100., 100., 1.0};  //light's position

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0, cam_hgt, 300, 0, 0, 0, 0, 1, 0);
    gluLookAt(0, cam_hgt, cam_dist, 0, 0, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    // gluLookAt (-80, cam_hgt, 180, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(angle, 0.0, 1.0, 0.0);		//rotate the whole scene

    glPushMatrix();
        drawFloor();
    glPopMatrix();

    glColor3f(1, 0, 0);
    glPushMatrix();
        if (!robots[0]->dead) {
            glPushMatrix();
            glTranslatef(-0.25*castle->getLength(), castle->getHeight(), 0.5*castle->getLength()+5);
            glRotatef(90, 0, 1, 0);
            glScalef(ROBOT_SCALE, ROBOT_SCALE, ROBOT_SCALE);
            glPushMatrix();
            glRotatef(-90, 0, -1, 0);
            float dir[] = {-1, -1, 0};
            float spotlight[] = {-robots[0]->deltaZ-10, robots[0]->deltaY, robots[0]->deltaX+10, 1.0f};
           // glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
            //glLightfv(GL_LIGHT1, GL_POSITION, spotlight);
            glPopMatrix();
                robots[0]->drawRobot();
            glPopMatrix();
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.5*castle->getLength(), castle->getHeight(), 0.5*castle->getLength()+5);
        glRotatef(180, 0, 1, 0);
        glScalef(ROBOT_SCALE, ROBOT_SCALE, ROBOT_SCALE);
        if (!robots[1]->dead) {
            robots[1]->drawRobot();
        }
    glPopMatrix();

    glPushMatrix();
        castle->drawCastle();
    glPopMatrix();

    glPushMatrix();
        cannons[0]->drawCannon();
    glPopMatrix();

    glPushMatrix();
   //     spaceships[0]->drawSpaceship();
    glPopMatrix();

    glutSwapBuffers();
}


//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

   // glEnable(GL_LIGHTING);					//Enable OpenGL states
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

//
//    float grey[4] = {0.2, 0.2, 0.2, 1.0};
//    float white[4]  = {1.0, 1.0, 1.0, 1.0};
//    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
//    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
//
//    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
//    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,1);
//
//    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, grey);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, grey);
//

  //  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//    glEnable(GL_COLOR_MATERIAL);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_NORMALIZE);
//    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glClearColor(0, 0, 0, 0);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

    glEnable(GL_LIGHTING);					//Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90, 1.78, 5, 1000);  //The camera view volume FOR TEST
    //gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection FOR PROD

}


//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (600, 600);

    glutInitWindowPosition (10, 10);
    glutCreateWindow ("Main");
    initialize();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    for (int i=0; i < 2; i++) {
        robots[i] = new Robots();
        robots[i]->patrolDistance = castle->getLength();
    }

    castle->yLevel = castle->getHeight() / 2;
    for(int i=0; i < 4;i++) {
        castle->walls[i]->color[0] = 1.0f;
        castle->walls[i]->color[1] = 0.65f;
        castle->walls[i]->color[2] = 0.0f;
        castle->walls[i]->color[3] = 1.0f;
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

    glutTimerFunc(100, idleAnim, 0);
    glutTimerFunc(100, patrolAnim, 1);

    spaceships[0] = new Spaceship();
    spaceships[0]->x = 0; spaceships[0]->y = 1.5*spaceships[0]->getRadius(); spaceships[0]->z = 0;
    spaceships[0]->setRadius(20);

    glutMainLoop();
    return 0;
}
