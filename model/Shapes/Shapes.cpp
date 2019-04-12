/**
 * @author George Khella
 * @date March 2019
 * @brief Implementation of shapes:
 *        sphere, ellipse, cone, pyramid, cylinder, cube
 */

#include "Shapes.h"


/**
 * Draw a sphere of given radius
 * @brief drawSphere
 * @param radius
 */
void drawSphere(float radius) {
    float alphaRadians = 0;
    float deltaRadians = 0;
    float grad = 10;
    float x, y, z;  // coordinates of point
    for (int alpha=0; alpha <= 360; alpha+=grad) {
        alphaRadians = alpha * M_PI / 180.0f;
        glBegin(GL_QUAD_STRIP);
        for (int delta=0; delta <= 180; delta+=grad) {
            deltaRadians = delta * M_PI / 180.0f;
            x = radius * sin(alphaRadians) * cos(deltaRadians);
            y = radius * sin(alphaRadians) * sin(deltaRadians);
            z = radius * cos(alphaRadians);
            glNormal3f(x, y, z);
            glVertex3f(x, y, z);

            x = radius * cos(deltaRadians) * sin(alphaRadians + (M_PI/grad));
            y = radius * sin(alphaRadians + (M_PI/grad)) * sin(deltaRadians);
            z = radius * cos(alphaRadians+(M_PI/grad));
            glNormal3f(x, y, z);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
}


/**
 * Draw a cylinder by creating two circles (base and top)
 * and a stacked circles to generate the 'tube'
 * @param radius
 * @param height
 */
void drawCylinder(float radius, float height) {
    float alphaRadians = 0;
    float betaRadians = 0;
    float x, z;

    // Base circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0); // Centre of base circle
    for(int alpha=0; alpha<=360; alpha++) {
        alphaRadians = alpha * M_PI / 180;
        x = radius * cos(alphaRadians);
        z = radius * sin(alphaRadians);
        glNormal3f(x, 0, z);
        glVertex3f(x, 0, z);
    }
    glEnd();

    // Create the tube
    glBegin(GL_QUAD_STRIP);
        for(int alpha=0; alpha<=360; alpha++) {
            alphaRadians = alpha * M_PI / 180;
            x = radius * cos(alphaRadians);
            z = radius * sin(alphaRadians);
            glNormal3f(x, 0, z);
            glVertex3f(x, 0, z);
            glNormal3f(x, height, z);
            glVertex3f(x, height, z);
        }
    glEnd();

    // Top circle
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, height, 0); // Centre of top circle
        for(int alpha=0;alpha<=360;alpha++) {
            alphaRadians = alpha * M_PI / 180;
            x = radius * cos(alphaRadians);
            z = radius * sin(alphaRadians);
            glNormal3f(x, height, z);
            glVertex3f(x, height, z);
        }
    glEnd();
}


/**
 * Draws a cone by creating triangle fans from
 * the apex and then creating a circle from
 * triangle fans
 * @param radius
 * @param height
 */
void drawCone(float radius, float height) {
    float alphaRadians;
    float x, y, z;
    glBegin(GL_TRIANGLE_FAN);
        // Apex - only needs to be specified once
        glVertex3f(0, height, 0);
        for(int alpha=0;alpha<=360;alpha++) {
            alphaRadians = alpha * M_PI /180;
            x = radius * cos(alphaRadians);
            z = radius * sin(alphaRadians);
            glNormal3f(x, 0, z);
            glVertex3f(x, 0, z);
        }
    glEnd();

    // Create the base circle
    glBegin(GL_TRIANGLE_FAN);
        // Centre of circle - only specify once
        glVertex3f(0, 0, 0);
        for(int alpha=0; alpha<=360; alpha++) {
            alphaRadians = alpha * M_PI / 180;
            x = radius * cos(alphaRadians);
            z = radius * sin(alphaRadians);
            glNormal3f(x, 0, z);
            glVertex3f(x, 0, z);
        }
    glEnd();
}


/**
 * Draws a square pyramid
 * @param baseLength
 */
void drawPyramid(float baseLength) {
    float halfLength = baseLength * 0.5;

    //Draw the base
    glBegin(GL_QUADS);
    glVertex3f(halfLength, 0, halfLength);
    glVertex3f(halfLength, 0, -halfLength);
    glVertex3f(-halfLength, 0, -halfLength);
    glVertex3f(-halfLength, 0, halfLength);
    glEnd();

    //Draw the triangles
    glBegin(GL_TRIANGLES);
    //Triangle 1
    glVertex3f(halfLength, 0, halfLength);
    glVertex3f(halfLength, 0, -halfLength);
    glVertex3f(0, halfLength, 0);

    //Triangle 2
    glVertex3f(halfLength, 0, -halfLength);
    glVertex3f(-halfLength, 0, -halfLength);
    glVertex3f(0, halfLength, 0);

    //Triangle 3
    glVertex3f(-halfLength, 0, -halfLength);
    glVertex3f(-halfLength, 0, halfLength);
    glVertex3f(0, halfLength, 0);

    //Triangle 4
    glVertex3f(-halfLength, 0, halfLength);
    glVertex3f(halfLength, 0, halfLength);
    glVertex3f(0, halfLength, 0);
    glEnd();
}


/**
 * Draw a cube given a length
 * @param length
 */
void drawCube(float length) {
    float halfLength = length * 0.5;
    glBegin(GL_QUADS);

    glVertex3f(-halfLength, halfLength, 0);
    glVertex3f(halfLength, halfLength, 0);
    glVertex3f(halfLength, -halfLength, 0);
    glVertex3f(-halfLength, -halfLength, 0);

    glVertex3f(-halfLength, halfLength, length);
    glVertex3f(halfLength, halfLength, length);
    glVertex3f(halfLength, -halfLength, length);
    glVertex3f(-halfLength, -halfLength, length);

    glVertex3f(-halfLength, halfLength, 0);
    glVertex3f(-halfLength, halfLength, length);
    glVertex3f(-halfLength, -halfLength, length);
    glVertex3f(-halfLength, -halfLength, 0);

    glVertex3f(halfLength, halfLength, 0);
    glVertex3f(halfLength, halfLength, length);
    glVertex3f(halfLength, -halfLength, length);
    glVertex3f(halfLength, -halfLength, 0);

    glVertex3f(-halfLength, halfLength, 0);
    glVertex3f(-halfLength, halfLength, length);
    glVertex3f(halfLength, halfLength, length);
    glVertex3f(halfLength, halfLength, 0);

    glVertex3f(-halfLength , -halfLength, length);
    glVertex3f(-halfLength, -halfLength, 0);
    glVertex3f(halfLength, -halfLength, 0);
    glVertex3f(halfLength, -halfLength, length);

    glEnd();

}


/**
 * Draw an ellipse
 * @param radiusHeight
 * @param radiusWidth
 */
void drawEllipse(float radiusHeight, float radiusWidth) {
    float alphaRadians = 0;
    float x, z;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for(int alpha=0; alpha<=360; alpha++) {
        alphaRadians = alpha * M_PI / 180;

        x = radiusWidth * cos(alphaRadians);
        z = radiusHeight * sin(alphaRadians);
        glNormal3f(x, 0, z);
        glVertex3f(x, 0, z);
    }
    glEnd();
}
