//
// Created by George on 1/04/2019.
//

#ifndef SPACESHIP_H
#define SPACESHIP_H
#define FLIGHT_TIME 180 // Time before spaceship hovers
#define FLAMES 50    // Fire particles per booster

#include <GL/freeglut.h>

class Spaceship {

private:
    double radius = 50, bodyHeight = 150;
    double boosterRadius = 20;
    void checkAlive();
    GLuint texIds[2];

public:
    double getRadius();
    double getBodyHeight();
    int getHealth();
    void setRadius(double radius);
    void takeDamage(); //Default damage amount
    void takeDamage(int damage);
    void loadTex();
    void drawBoosters();
    void drawFlameParticle();
    void drawFlames();
    void drawWingTips();
    void drawSpaceship();
    bool isGrounded();
    bool isFlying();
    struct AnimValues {
        int takeOffValue;
        bool grounded;
        bool flying;
        float x, y, z;
    };
    bool texture;

    static struct AnimValues animValues;

    static void takeOffAnim(int value);
    static void destroyAnim(int value);

    Spaceship(int radius);
    Spaceship() {};
};


#endif //ASSIGNMENT1_SPACESHIP_H
