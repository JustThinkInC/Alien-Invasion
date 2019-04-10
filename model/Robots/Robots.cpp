/**
 * @author George K
 * @date 6/03/2019
 * @brief Implementation of Robots class methods
 */

#include "Robots.h"
#include "../Shapes/Shapes.h"
#include <math.h>
#include <GL/freeglut.h>
using namespace std;

Robots *robots[2];

void Robots::drawRobot() {
    // Rotate in place by step when turning
    // otherwise rotate 180 by y-axis
    if (turning) {
        glTranslatef(0, 0, deltaZ);
        glRotatef(turnAngle * turnStep, 0, 1, 0);
        glTranslatef(0, 0, -deltaZ);
    } else if (invert) {
        glTranslatef(0, 0, deltaZ);
        glRotatef(180, 0, 1, 0);
        glTranslatef(0, 0, -deltaZ);
    }

    // Head
    glPushMatrix();
        glTranslatef(0 + deltaX, 140 + deltaY, 0 + deltaZ);
        glColor4f(1, 0, 0, alpha);
        drawSphere(20);
    glPopMatrix();

    // Right eye
    glPushMatrix();
        glTranslatef(10 + deltaX, 140 + deltaY, 19 + deltaZ);
        glRotatef(90, 1, 0, 0);
        glColor4f(1, 1, 0, alpha);
        drawEllipse(4, 2);
    glPopMatrix();

    // Left eye
    glPushMatrix();
        glTranslatef(-10 + deltaX, 140 + deltaY, 19 + deltaZ);
        glColor4f(1, 1, 0, alpha);
        glRotatef(90, 1, 0, 0);
        drawEllipse(4, 2);
    glPopMatrix();

    //Neck
    glPushMatrix();
        glTranslatef(0 + deltaX, 110 + deltaY, 0 + deltaZ);
        glColor4f(0.46, 0.46, 0.46, alpha);
        drawCylinder(5, 25);
    glPopMatrix();

    // Body
    glPushMatrix();
        glTranslatef(0 + deltaX, 110 + deltaY, 0 + deltaZ);
        glColor4f(1, 0.6, 0, alpha);
        glScalef(1, -0.75, 1);
        drawCone(20, 100);
    glPopMatrix();

    //Right Shoulder
    glPushMatrix();
        glTranslatef(-25 + deltaX, 105 + deltaY, 0 + deltaZ);
        glColor4f(0.46, 0.46, 0.46, alpha);
        drawSphere(10);
    glPopMatrix();

    //Left Shoulder
    glPushMatrix();
        glTranslatef(25 + deltaX, 105 + deltaY, 0 + deltaZ);
        glColor4f(0.46, 0.46, 0.46, alpha);
        drawSphere(10);
    glPopMatrix();


    // Upper right arm
    glPushMatrix();
        glTranslatef(-25 + deltaX, 70 + deltaY, 0 + deltaZ);
        glColor4f(1, 0, 0, alpha);
        drawCylinder(5, 30);
    glPopMatrix();


    // Upper left arm
    glPushMatrix();
        glTranslatef(25 + deltaX, 70 + deltaY, 0 + deltaZ);
        glColor4f(0, 1, 0, alpha);
        drawCylinder(5, 30);
    glPopMatrix();

    // Right Elbow
    glPushMatrix();
        glTranslatef(-25 + deltaX, 65 + deltaY, 0 + deltaZ);
        glColor4f(0, 0, 1, alpha);
        drawSphere(7);
    glPopMatrix();

    // Left Elbow
    glPushMatrix();
        glTranslatef(25 + deltaX, 65 + deltaY, 0 + deltaZ);
        glColor4f(0, 0, 0, alpha);
        drawSphere(7);
    glPopMatrix();

    // Right arm
    glPushMatrix();
        glTranslatef(-25 + deltaX, 65 + deltaY, 0 + deltaZ);
        glColor4f(0.5, 0.5, 0.5, alpha);
        glRotatef(90, 1, 0, 0);
        drawCylinder(5, 30);
    glPopMatrix();

    // Left arm
    glPushMatrix();
        glTranslatef(25 + deltaX, 65 + deltaY, 0 + deltaZ);
        glColor4f(0.25, 0.25, 0.25, alpha);
        glRotatef(90, 1, 0, 0);
        drawCylinder(5, 30);
    glPopMatrix();

    // Leg link
    glPushMatrix();
        glTranslatef(0 + deltaX, 50 + deltaY, 0 + deltaZ);
        glColor4f(0.46, 0.46, 0.46, alpha);
        glScalef(1, -1, 1);
        drawCylinder(5, 20);
    glPopMatrix();

    // Leg ball
    glPushMatrix();
        glTranslatef(0 + deltaX, 20 + deltaY, 0 + deltaZ);
        glColor4f(0.46, 0.46, 0.46, alpha);
        drawSphere(18);
    glPopMatrix();

}


/**
 * Minor up down movement of robot
 * Imitates a breathing sort of movement
 * @param value
 */
void idleAnim(int index) {
    Robots *robot = robots[index];

    if (robot->moving || robot->turning) return;
    robot->idle = true;

    int *value = &robot->animValues.idleValue;
    int time = 10;

    if (0 <= *value && *value <= time) {
        robot->deltaY++;
        *value = (*value + 1 > time) ? -time : *value + 1;
    } else if (*value < 0) {
        robot->deltaY--;
        *value += 1;
    }

    glutPostRedisplay();
    glutTimerFunc(100, idleAnim, index);
}


/**
* Turn the robot around 180 degrees smoothly
* by increasing the turn step
* @param value
*/
void turnAroundAnim(int index) {
    static int time = 20;

    Robots *robot = robots[index];
    robot->turnAngle = 180 / time;
    int *value = &robot->animValues.turnAroundValue;

    if (*value < time && robot->turnStep < time) {
        *value += 1;
        if (robot->invert) robot->turnStep++; else robot->turnStep--;
        glutPostRedisplay();
        glutTimerFunc(100, turnAroundAnim, index);
    } else {
        robot->turning = false;
        robot->turnStep = -time; // Set to inverse for next turn
        // Run the idle animation if not robot is not moving
        if (!robot->moving) {
            *value = 0;
            glutTimerFunc(100, idleAnim, index);
        }
    }

    glutPostRedisplay();
}


/**
* Perform a patrol by increasing or decreasing
* the deltaZ value. deltaY is decreased/increased
* slightly for more realistic movement
* @param value
*/
void patrolAnim(int index) {
    static int patrolTime = 100;   // 10 seconds
    Robots *robot = robots[index];

    // Dead or dying robots cannot patrol
    if (robot->dead || robot->dying) return;

    int *value = &robot->animValues.patrolValue;

    // If the robot has turned twice (i.e. completed a patrol)
    // then stop the animation
    if (robot->turns == 2) {
        robot->moving = false;
        robot->idle = true;
        robot->deltaY = 0;
        robot->deltaZ = 0;
        robot->turns = 0;
        robot->animValues.patrolValue = 0;
        glutTimerFunc(100, idleAnim, index);

        return;
    } else {
        robot->moving = true;
        robot->idle = false;
    }

    if (0 <= *value && *value <= (0.5 * patrolTime)) {
        if (robot->invert) {
            robot->turning = true;
            robot->animValues.turnAroundValue = 0;
            glutTimerFunc(100, turnAroundAnim, index);
            robot->invert = false;
            robot->turns++;
        }
        if (!robot->turning) {
            robot->deltaZ += robot->patrolDistance * 3 / (0.5 * patrolTime);
            *value = (*value + 1 > (0.5 * patrolTime)) ? -(0.5 * patrolTime) : *value + 1;
        }
    } else if (*value < 0) {
        if (!robot->invert) {
            robot->turning = true;
            robot->animValues.turnAroundValue = 0;
            glutTimerFunc(100, turnAroundAnim, index);
            robot->invert = true;
            robot->turns++;
        }
        if (!robot->turning) {
            robot->deltaZ -= robot->patrolDistance * 3 / (0.5 * patrolTime);
            *value += 1;
        }
    }

    robot->deltaY = (robot->deltaY >= 0) ? -0.5 : 0; // Small bobbing movement for realism
    glutPostRedisplay();
    glutTimerFunc(100, patrolAnim, index);
}


/**
* Dying robot animation
* The robot fades till invisible
* @param value
*/
void dieAnim(int index) {
    Robots *robot = robots[index];
    int *value = &robot->animValues.dieValue;

    robot->idle = false;
    robot->moving = true;

    if (*value < 10) {
        *value += 1;
        robot->turning = true;
        robot->turnAngle = 360;
        robot->animValues.turnAroundValue = -20;
        glutTimerFunc(0, turnAroundAnim, index);
    } else {
        *value += 1;
    }

    if (*value < 20) {
        robot->dying = true;
        // Increase fade -- robot dying
        robot->alpha = (robot->alpha - 0.05 >= 0) ? robot->alpha - 0.05 : 0;
        robot->dead = false;
    } else {
        robot->dying = false;
        robot->dead = true;
    }

    glutPostRedisplay();
    glutTimerFunc(100, dieAnim, index);
}