//
// Created by George on 6/03/2019.
//

#ifndef CANNON_H
#define CANNON_H


class Cannon {
private:
    const int length = 60;
    float *x, *y, *z;  //vertex coordinate arrays
    int *t1, *t2, *t3; //triangles
    int nvrt, ntri;    //total number of vertices and triangles

public:
    float cannonX, cannonY, cannonZ, tilt = 0;
    double cannonBallX = 0, cannonBallY = 0, sy = -1;
    bool smoke = false;
    bool firing = false;


    struct AnimValues {
        int firingValue = 0;
    };

    AnimValues animValues = {0};

    void loadMeshFile(const char* fname);
    void normal(int tindx);
    void drawCannon();
    int getLength();

    Cannon() {}
};

extern Cannon* cannons[4];

void fireCannonAnim(int index);

void keyboard(unsigned char key, int x, int y);

void special(int key, int x, int y);


#endif //LAB1_CANNON_H
