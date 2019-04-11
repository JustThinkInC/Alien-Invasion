/**
 * @author George K
 * @date 6/03/2019
 * @brief Header for a simple castle with walls,
 *        pillars and gates. Each has a specified amount
 *        of health
 */
#ifndef CASTLE_H
#define CASTLE_H
#include <GL/freeglut.h>

class Castle {
private:
    int length;
    int height;
    int columns;
    GLuint texIds[2];
    void drawGate(double length);
    void drawWall();
    void normal(float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float x3, float y3, float z3 );

public:
    Castle(int length, int height);
    struct Wall {
        double length, x, y, z;
    };

    struct Gate {
        double length, width, x, y, z;
        bool open = false, opening = false, closed = true, closing = false;
        int angle = 0;
    };

    double xFront, yFront, zFront; //Coordinates of front wall

    double yLevel; //Distance above ground
    static struct Wall* walls[5];
    static struct Gate gate;

    void loadTex();

    int getLength();

    int getHeight();

    void drawPillars(bool triangular);

    void drawCastle();

    static void openGateAnim(int value);

    static void closeGateAnim(int value);
};

#endif //CASTLE_H
