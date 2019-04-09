//
// Created by George on 1/04/2019.
//

#ifndef SPACESHIP_H
#define SPACESHIP_H
#define TAKE_OFF_TIME 100
#define FLAMES 50    // Fire particles per booster

#include <GL/freeglut.h>

class Spaceship {

private:
    int health = 100, radius = 50;
    float boosterRadius = 20;
    void checkAlive();
    GLuint texIds[2];

public:
    int getRadius();
    int getHealth();
    void setRadius(int radius);
    void takeDamage(); //Default damage amount
    void takeDamage(int damage);
    void loadTex();
    void drawBoosters();
    void drawFlameParticle();
    void drawFlames();
    void drawWingTips();
    void drawSpaceship();
    bool isGrounded();

    struct AnimValues {
        int idleValue;
        int takeOffValue;
        int dieValue;
        int boosterValue;
        bool grounded;
        float x, y, z;
    };

    static struct AnimValues animValues;

    static void takeOffAnim(int value);
    static void destroyAnim(int value);

    Spaceship(int radius);
    Spaceship() {};
};


#endif //ASSIGNMENT1_SPACESHIP_H
