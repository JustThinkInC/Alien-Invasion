//
// Created by George on 1/04/2019.
//

#ifndef SPACESHIP_H
#define SPACESHIP_H
#define TAKE_OFF_TIME 10

class Spaceship {

public:
    float x, y, z; // Coordinates

    int getRadius();
    int getHealth();
    void setRadius(int radius);
    void takeDamage(); //Default damage amount
    void takeDamage(int damage);
    void drawSpaceship();
    bool isGrounded();

    struct AnimValues {
        int idleValue = 0;
        int takeOffValue = 0;
        int dieValue = 0;
    };

    AnimValues animValues = {0};

    Spaceship() {};

private:
    int health = 100, radius = 50;
    bool grounded = true;
    void checkAlive();
};

extern Spaceship* spaceships[1];

void takeOffAnim(int index);
void destroyAnim(int index);

#endif //ASSIGNMENT1_SPACESHIP_H
