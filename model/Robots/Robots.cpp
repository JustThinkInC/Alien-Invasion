/**
 * @author George K
 * @date 6/03/2019
 * @brief Implementation of Robots class methods
 */

#include "Robots.h"
#include "../Shapes/Shapes.h"
#include "../Wineglass/Wineglass.h"
#include <math.h>
#include <GL/freeglut.h>
using namespace std;

Robots *robots[4];

/**
 * Draw the robot
 */
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
        glTranslated(-25 + deltaX, 105+deltaY, deltaZ);
        glRotatef(animValues.upperRightArmAngle, -1, 0, 0);
        glTranslatef(-(-25+deltaX), -(105+deltaY), -deltaZ);
        glTranslatef(-25+deltaX, 70+deltaY, deltaZ);
        glColor4f(1, 0, 0, alpha);
        drawCylinder(5, 30);
    glPopMatrix();


    // Upper left arm
    glPushMatrix();
        glTranslated(25 + deltaX, 105+deltaY, deltaZ);
        glRotatef(animValues.upperLeftArmAngle, -1, 0, 0);
        glTranslated(-25+deltaX, -(105+deltaY), -deltaZ);
        glTranslatef(25 + deltaX, 70 + deltaY, 0 + deltaZ);
        glColor4f(0, 1, 0, alpha);
        drawCylinder(5, 30);
    glPopMatrix();

    // Right Elbow
    glPushMatrix();
        glTranslated(-25 + deltaX, 105+deltaY, deltaZ);
        glRotatef(animValues.rightElbowAngle, -1, 0, 0);
        glTranslated(-(-25+deltaX), -(105+deltaY), -deltaZ);
        glTranslatef(-25 + deltaX, 65 + deltaY, 0 + deltaZ);
        glColor4f(0, 0, 1, alpha);
        drawSphere(7);
    glPopMatrix();

    // Left Elbow
    glPushMatrix();
        glTranslated(25 + deltaX, 105 + deltaY, deltaZ);
        glRotatef(animValues.leftElbowAngle, -1, 0, 0);
        glTranslatef(-25 + deltaX, -(105 + deltaY), -deltaZ);

        glTranslatef(25 + deltaX, 65 + deltaY, 0 + deltaZ);
        glColor4f(0, 0, 0, alpha);
        drawSphere(7);
    glPopMatrix();

    // Right arm
    glPushMatrix();
        if (animValues.upperRightArmAngle >= 90) {
            glTranslatef(2.5 + deltaX, 100 + deltaY, 15 + deltaZ);
            glRotatef(100, -1, 0, -1);
            glTranslatef(-(-25+deltaX), -(105 + deltaY), -deltaZ);

        } else {
            glTranslatef(-25 + deltaX, 105 + deltaY, 0 + deltaZ);
            glRotatef(animValues.rightArmAngle, -1, 0, 0);
            glTranslatef(-(-25+deltaX), -(105 + deltaY), -deltaZ);
        }
        glTranslatef(-25 + deltaX, 65 + deltaY, 0 + deltaZ);

        glColor4f(0.5, 0.5, 0.5, alpha);
        glRotatef(90, 1, 0, 0);
        drawCylinder(5, 30);
    glPopMatrix();

    if (drink) {
        glPushMatrix();
            if (animValues.upperRightArmAngle >= 90) {
                glTranslated(5 + deltaX, 145 + deltaY, 5 + deltaZ);
                glRotatef(40, -1, 0, -1);
                glTranslatef(-(-25 + deltaX), -(105 + deltaY), -deltaZ);
            } else {
                glTranslated(-25 + deltaX, 105 + deltaY, deltaZ);
                glRotatef(animValues.upperRightArmAngle, -1, 0, 0);
                glTranslatef(-(-25 + deltaX), -(105 + deltaY), -deltaZ);
            }

            glTranslated(-25 + deltaX, 55 + deltaY, deltaZ + 30);
            glScalef(2, 2, 2);

            glColor4f(0.8, 0.93, 1, 0.35);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawWineGlass(true);
            glDisable(GL_BLEND);
        glPopMatrix();
    }


    // Left arm
    glPushMatrix();
        glTranslated(25 + deltaX, 105+deltaY, deltaZ);
        glRotatef(animValues.leftElbowAngle, -1, 0, 0);
        glTranslated(-25+deltaX, -(105+deltaY), -deltaZ);
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
 * Make robot drink
 * @param index
 */
void drinkAnim(int index) {
    Robots *robot = robots[index];
    if (robot->animValues.upperRightArmAngle < 90) {
        robot->animValues.upperRightArmAngle += 2;
        robot->animValues.rightArmAngle += 2;
        robot->animValues.rightElbowAngle += 2;
        robot->drinking = true;
        robot->stopDrinking = false;
        robot->animValues.drinkValue += 2;
    } else {
        robot->animValues.drinkValue = 90;
        robot->drinking = false;
        glutPostRedisplay();
        return;
    }

    glutPostRedisplay();
    glutTimerFunc(50, drinkAnim, index);
}


/**
 * Cease drinking
 * @param index
 */
void stopDrinkAnim(int index) {
    Robots *robot = robots[index];
    if (robot->animValues.upperRightArmAngle > 0) {
        robot->animValues.upperRightArmAngle -= 2;
        robot->animValues.rightArmAngle -= 2;
        robot->animValues.rightElbowAngle -= 2;
        robot->drinking = false;
        robot->stopDrinking = true;
        robot->animValues.drinkValue -= 2;
    } else {
        robot->animValues.drinkValue = 0;
        robot->stopDrinking = false;
        robot->drinking = false;
        glutPostRedisplay();
        return;
    }

    glutPostRedisplay();
    glutTimerFunc(50, stopDrinkAnim, index);
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
        glutTimerFunc(100, turnAroundAnim, index);
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


/**
 * Make robot dance
 * @param index
 */
void danceAnim(int index) {
    Robots* robot = robots[index];
    int *value = &robot->animValues.danceValue;

    if (* value < 100) {
        robot->deltaX = rand() % 5 - 5;
        robot->deltaY = rand() % 10;
        robot->deltaZ = rand() % 15;

        robot->animValues.upperRightArmAngle = rand() % 90 - 45;
        robot->animValues.rightArmAngle = robot->animValues.upperRightArmAngle;
        robot->animValues.rightElbowAngle = robot->animValues.upperRightArmAngle;

        robot->animValues.upperLeftArmAngle = rand() %  90 - 45;
        robot->animValues.leftArmAngle = robot->animValues.upperLeftArmAngle;
        robot->animValues.leftElbowAngle = robot->animValues.upperLeftArmAngle;

        robot->turnAngle = rand() % 360;
        robot->turnStep = 1;
        robot->turning = true;
        *value = *value + 1;
        robot->dancing = true;
    } else {
        *value = 0;
        robot->deltaX = 0;
        robot->deltaY = 0;
        robot->deltaZ = 0;
        robot->dancing = false;
        robot->animValues.upperRightArmAngle = 0;
        robot->animValues.rightArmAngle = 0;
        robot->animValues.rightElbowAngle = 0;
        robot->animValues.upperLeftArmAngle = 0;
        robot->animValues.leftArmAngle = 0;
        robot->animValues.leftElbowAngle = 0;
        robot->turnAngle = 0;
        robot->turnStep = 0;
        robot->turning = false;
        glutPostRedisplay();
        return;
    }

    glutPostRedisplay();
    glutTimerFunc(100, danceAnim, index);
}
