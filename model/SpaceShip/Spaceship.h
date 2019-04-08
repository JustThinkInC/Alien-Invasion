//
// Created by George on 1/04/2019.
//

#ifndef SPACESHIP_H
#define SPACESHIP_H
#define TAKE_OFF_TIME 10

#include <GL/freeglut.h>

class Spaceship {

private:
    int health = 100, radius = 50;
    void checkAlive();
    GLuint wingTex;

public:
    int getRadius();
    int getHealth();
    void setRadius(int radius);
    void takeDamage(); //Default damage amount
    void takeDamage(int damage);
    void loadTex();
    void drawWingTips();
    void drawSpaceship();
    bool isGrounded();

    struct AnimValues {
        int idleValue;
        int takeOffValue;
        int dieValue;
        bool grounded;
        float x, y, z;
    };

    static struct AnimValues animValues;

    static void takeOffAnim(int index);
    static void destroyAnim(int index);

    Spaceship(int radius);
    Spaceship() {};
};


#endif //ASSIGNMENT1_SPACESHIP_H
