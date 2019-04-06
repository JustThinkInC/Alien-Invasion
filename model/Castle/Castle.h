/**
 * @author George K
 * @date 6/03/2019
 * @brief Header for a simple castle with walls,
 *        pillars and gates. Each has a specified amount
 *        of health
 */
#ifndef CASTLE_H
#define CASTLE_H

class Castle {
private:
    int length;
    int height;
    int columns;

public:
    Castle(int length, int height);
    struct Wall {
        int health = 100;
        bool destroyed = false;
        int number;
        int length;
        float color[4] = {0, 0, 0, 1};
    };

    struct Gate {
        int health = 100;
        bool destroyed = false, open = false, opening = false, closed = true, closing = false;
        float color[4] = {0.5, 0.5, 0.5, 1};
        float angle = 0;
    };

    float xFront, yFront, zFront; //Coordinates of front wall

    float yLevel; //Distance above ground
    static struct Wall* walls[5];
    static struct Gate gate;

    void setColumns(int columns);

    int getColumns();

    void setLength(int length);

    int getLength();

    int getHeight();

    void destroyWall(Wall* wall);

    void drawCastle();

    void damageWall(int damage, Wall* wall);

    static void fadeWallAnim(int wallNum);

    static void openGateAnim(int value);

    static void closeGateAnim(int value);
};

#endif //CASTLE_H
