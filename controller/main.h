#ifndef MAIN_H
#define MAIN_H
#define ROBOT_SCALE 0.4

static float angle = 0.0;  //Rotation angle for viewing
static double eyeX = 0, eyeY = 0, eyeZ = 0, lookX = 0, lookY = 0, lookZ = -1, stepSpeed = 20;
static bool spaceView = false, fullscreen = false;
static double minBoundary = -950, maxBoundary = 950;
static float lpos[4] = {-1000., 1000., -1000., 1.0};  //light's position


void drawFloor();
void display();
void renderSpaceship();
bool collision(bool down);
bool gateOpenable();
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void initObjects();
void initialize();

#endif //MAIN_H
