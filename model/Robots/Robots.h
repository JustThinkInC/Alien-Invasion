/**
 * @author George K
 * @date 6/03/2019
 * @brief Robots class header file
 */

#ifndef ROBOTS_H
#define ROBOTS_H

using namespace std;
class Robots {
public:
    // Robot position changes -- use for animations
    double deltaX = 0, deltaY = 0, deltaZ = 0, turnStep = 0,
            turnAngle = 0, alpha = 1, patrolDistance = 0;

    // Robot states
    bool moving = false, turning = false, idle = false, invert = false, dying = false, dead = false, drinking = false,
    drink = false, stopDrinking = false, dancing = false;

    // Turns made by robot, 2 turns equals one patrol
    int turns = 0;

     struct AnimValues {
         int idleValue;
         int patrolValue;
         int turnAroundValue;
         int dieValue;
         int drinkValue;
         int danceValue;
         double rightElbowAngle, rightArmAngle, upperRightArmAngle, leftElbowAngle, leftArmAngle, upperLeftArmAngle;
     };

      // Width is Head radius + body radius + shoulder diameter
      // Height is Head radius + neck length + body length + leg link length + leg ball radius
     double height = 183, width = 60, x, y, z;

     AnimValues animValues = {0};
     Robots() {}

     void drawRobot();   // Draws the actual robot
};

extern Robots* robots[4];

void idleAnim(int index);   // Animation for idle state

void patrolAnim(int index); // Animation for patrolling

void turnAroundAnim(int index); // Animation for turning robot around

void dieAnim(int index); // Animation for dying robot

void drinkAnim(int index);

void stopDrinkAnim(int index);

void danceAnim(int index);

#endif //ROBOTS_H
