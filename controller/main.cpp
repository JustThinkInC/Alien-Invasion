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
static double eyeX = 0, eyeY = 0, eyeZ = 0, lookX = 0, lookY = 0, lookZ = -1, stepSpeed = 10;
static bool spaceView = false;
static double minBoundary = -950, maxBoundary = 950;

Castle* castle;
Skybox* skybox;
Spaceship* spaceship;


void drawFloor()
{
    float white[4] = {1., 1., 1., 1.};
    float black[4] = {0};
    //glColor4d(0.13, 0.098, 0.043, 1.0);  //The floor is gray in colour
    glColor4f(0.7, 0.7, 0.7, 1.0);
    glNormal3f(0.0, 1.0, 0.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

    //The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
    glBegin(GL_QUADS);
    for(int i = -1000; i < 1000; i++)
    {
        for(int j = -1000;  j < 1000; j++)
        {
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glVertex3f(i+1, 0, j+1);
            glVertex3f(i+1, 0, j);
        }
    }
    glEnd();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}


void display()
{
    float lpos[4] = {-1000., 1000., -1000., 1.0};  //light's position


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
        glTranslated(-0.5*castle->getLength(), 0, 0.5*castle->getLength()+robots[0]->width-200);
        glRotatef(90, 0, 1, 0);
        glScalef(ROBOT_SCALE, ROBOT_SCALE, ROBOT_SCALE);

        if (!robots[0]->dead) {
            robots[0]->drawRobot();
            robots[0]->x = -0.5 * castle->getLength() + robots[0]->deltaZ; // Robot's path is in Z plane but here we rotate it to X plane
            robots[0]->z = 0.5*castle->getLength()+robots[0]->width-200;
        }
        // Spotlight
        float spotlight[] = {robots[0]->z, robots[0]->height, robots[0]->x+2*robots[0]->z+2*robots[0]->width, 1.0f};
        float dir[] = {-1, -1, 0.0};
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
        glLightfv(GL_LIGHT1, GL_POSITION, spotlight);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.5*castle->getLength(), castle->getHeight(), 0.5*castle->getLength()+5);
        glRotatef(180, 0, 1, 0);
        glScalef(ROBOT_SCALE, ROBOT_SCALE, ROBOT_SCALE);
        if (!robots[1]->dead) {
            robots[1]->drawRobot();
            robots[1]->x = 0.5 * castle->getLength() + robots[1]->deltaX;
            robots[1]->z = 0.5 * castle->getLength() + 5 + robots[1]->deltaZ;
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, -200);
        //castle->drawCastle();
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
}


/**
 * Check collision of player with scene objects
 * @param down If the player is moving back
 * @return boolean value of collision
 */
bool collisionCheck(bool down) {
    static int numRobots = sizeof(robots) / sizeof(robots[0]);
    double radAngle = angle * M_PI / 180;
    double newX = eyeX + stepSpeed * sin(radAngle);
    double newZ = eyeZ - stepSpeed * cos(radAngle);
    bool xCol;
    bool zCol;

    if (down) {
        newX = eyeX - stepSpeed * sin(radAngle);
        newZ = eyeZ + stepSpeed * cos(radAngle);
    }

    // Check collision with robots;
    for (int i = 0; i < numRobots; i++) {
        double posX = robots[i]->x;
        double width = robots[i]->width;
        double posZ = robots[i]->z;
        xCol = ((posX - width) == newX) || (newX == (posX + width));
        zCol = ((posZ - width) == newZ) || (newZ == (posZ + width));

        if (xCol && zCol) {
            cout << "-X " << robots[i]->x - robots[i]->width << " +X " << (robots[i]->x + robots[i]->width) << " newC" << newX << " oldC " << eyeX << " -Z " << (robots[i]->z - robots[i]->width) << " +Z " << (robots[i]->z + robots[i]->width) << endl;
            cout << "ROBOT COLLISION" << endl;
            return true;
        }
    }

    // Check collision with castle walls;
    for (int i = 0; i < 5; i++) {
        double wallX = castle->walls[i]->x;
        double wallZ = castle->walls[i]->z;
        double wallLength = castle->walls[i]->length;
        double halfLength = wallLength * 0.5;

        // Adjust collision check for left and right walls
        if (i == 2 || i == 4) {
            xCol = (wallX - 35 < newX) && (newX < wallX + 35);
            zCol = (wallZ - 200 - halfLength <= newZ) && (newZ <= wallZ + halfLength - 200);
        } else {
            xCol = (wallX - halfLength < newX) && (newX < wallX + halfLength);
            zCol = (wallZ - 200 - 35 <= newZ) && (newZ <= wallZ - 200 + 35);
        }

        if ( xCol && zCol) {
            return true;
        }
    }

    // Check collision with spaceship
    // Note that the player can move under the spaceship if it has taken off
    xCol = (-spaceship->getRadius() - 10 <= newX) && (newX <= spaceship->getRadius() + 10);
    zCol = (-spaceship->getRadius() - 200 - 10 <= newZ) && (newZ <= spaceship->getRadius() - 200 + 10);
    if (eyeY <= spaceship->getBodyHeight()) {
        if (xCol && zCol && spaceship->animValues.y <= eyeY) {
            return true;
        }
    }
    return false;
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


/**
 * Special key event call back
 * Allows arrow-key movement and toggling camera view between spaceship and normal
 * @param key
 * @param x
 * @param y
 */
void special(int key, int x, int y) {
    double radAngle = angle * M_PI / 180;

    if (key == GLUT_KEY_LEFT) {
        angle -= 5;
        radAngle = angle * M_PI/180;
    }
    else if (key == GLUT_KEY_RIGHT) {
        angle += 5;
        radAngle = angle * M_PI / 180;
    }
    else if (key == GLUT_KEY_UP && (glutGetModifiers() == GLUT_ACTIVE_SHIFT)) {
        eyeY += stepSpeed * cos(radAngle);
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        eyeY -= stepSpeed * cos(radAngle);
    }
    else if (key == GLUT_KEY_UP && !collisionCheck(false) && (eyeZ > minBoundary || (eyeX < maxBoundary && eyeX > minBoundary))) {
        eyeX += stepSpeed * sin(radAngle);
        eyeZ -= stepSpeed * cos(radAngle);
    }
    else if (key == GLUT_KEY_DOWN && !collisionCheck(true) && (eyeZ < maxBoundary || (eyeX < maxBoundary && eyeX > minBoundary))) {
        eyeX -= stepSpeed * sin(radAngle);
        eyeZ += stepSpeed * cos(radAngle);
    }
    else if (key == GLUT_KEY_HOME) {
        spaceView ^= true;  // Toggle states
    }


    // Ensure camera in bounds
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


/**
 * Initialise objects in scene
 */
void initObjects() {
    skybox = new Skybox();
    skybox->loadTextures();

    //castle = new Castle(300, 100);
    //castle = new Castle(450, 200);
    castle = new Castle(700, 300);
    castle->loadTex();

    // Create robots
    for (int i=0; i < 2; i++) {
        robots[i] = new Robots();
        robots[i]->patrolDistance = 0.8*castle->getLength();
        robots[i]->width *= ROBOT_SCALE;
        robots[i]->height *= ROBOT_SCALE;
    }

    // Create cannon
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


/**
 * Initialize OpenGL parameters
 */
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

    // Spotlight stuff
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    // New light source for spotlight
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 5, 5000);
    //gluPerspective(80.0, 1.0, 100.0, 5000.0);   //Perspective projection from Skybox lab

    initObjects();

    eyeZ = castle->getLength();
    eyeY = 5.5;
}


/**
 * Main: Initialize glut window and register call backs
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    //glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    glutInitWindowPosition(0, 0);
    glutCreateWindow ("Alien Invasion!");   // Assignment title
    initialize();

    //glutFullScreen();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    display();

    glutMainLoop();
    return 0;
}
