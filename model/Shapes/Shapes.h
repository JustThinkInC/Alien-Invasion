//
// Created by George on 9/03/2019.
//

#ifndef SHAPES_H
#define SHAPES_H
#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>

void drawCylinder(float radius, float height);
void drawCone(float radius, float height);
void drawCube(float length);
void drawSphere(float radius);
//void drawSphere(float radius, int stacks, int slices);
void drawEllipse(float radiusWidth, float radiusHeight);
void drawPyramid(float baseLength);


#endif //SHAPES_H
