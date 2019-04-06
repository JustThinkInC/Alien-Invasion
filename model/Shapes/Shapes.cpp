//
// Created by George on 9/03/2019.
//

#include "Shapes.h"

/*
 * Draws a sphere based on the parametric formula
 * x = radius * sin(alpha) * cos(delta)
 * y = radius * sin(alpha) * sin(delta)
 * z = radius * cos(alpha)
 * Where 0 <= alpha <= 360 degrees and 0 <= delta <= 180 degrees
 */
//void drawSphere(float radius) {
//    float alphaRadians = 0;
//    float deltaRadians = 0;
//    float k = 10;
//    float x, y, z;  // coordinates of point
//    glBegin(GL_QUAD_STRIP);
//    for (int alpha=0; alpha <= 360; alpha+=20) {
//        alphaRadians = alpha * M_PI / 180.0f;
//        x = radius * cos(alphaRadians);
//        y = k;
//        z = radius * sin(alphaRadians);
//
//        glVertex3f(x, 0, z);
//        glVertex3f(x, y+k, z);
//
//        //So rotate about y axis? Z seems to work better
//        //But both leading no where.
//    }
//    glEnd();
//
//}


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


/* Method with stacks and slices
//void drawSphere(float radius, int stacks, int slices) {
//    float alphaRadians = 0;
//    float x, y, z, nx, ny, nz;
//    //for(int alpha;alpha<=360;alpha++) {
//    for(int i = 0; i <= stacks; i++) {
//        //float lat = M_PI / stacks * i - (0.5 * M_PI);
//        float lat = i * M_PI / stacks;
//       // float nlat = M_PI / stacks * (i+1) - (0.5 * M_PI);
//        glBegin(GL_QUAD_STRIP);
//        for (int j=0; j <= slices; j++) {
//            float lon = 2 * j * M_PI / slices;
////            x = cos(lon) * cos(lat);
////            y = sin(lon) * cos(lat);
////            z = sin(lat);
////            nx = cos(lon) * cos(nlat);
////            ny = sin(lon) * cos(nlat);
////            nz = sin(nlat);
////            glNormal3f(nx, ny, nz);
////            glVertex3f(radius*nx, radius*ny, radius*nz);
////            glNormal3f(x, y, z);
////            glVertex3f(radius*x, radius*y, radius*z);
//              x = cos(lon) * sin(lat);
//              y = cos(lat);
//              z = sin(lon) * sin(lat);
//
//              glVertex3f(x*radius, y*radius, z*radius);
//              glVertex3f(radius * x, radius * y, radius * z);
//        }
//        glEnd();
//    }
//}*/

/*Inefficient parametric form
//void drawSphere(float radius) {
//    float x=0, y=0, z=0;
//    float alphaRadians = 0;
//    float deltaRadians = 0;
//    for(int alpha=0;alpha<=360;alpha++) {
//        alphaRadians = alpha * M_PI /180;
//        glBegin(GL_LINE_STRIP);
//
//        for(int delta=0;delta<=180; delta++) {
//            deltaRadians = delta * M_PI / 180;
//            x = radius * sin(alphaRadians) * cos(deltaRadians);
//            y = radius * sin(deltaRadians);
//            z = radius * cos(alphaRadians) * cos(deltaRadians);
//
//            glVertex3f(x, y, z);
//            glVertex3f(x, -y, z);
//            //glVertex3f(-x, -y, -z);
//        }
//        glEnd();
//    }
//}
*/


//void drawSphere(float radius) {
//    float thetaRadians = 0, deltaRadians = 0;
//    float x = 0, y = 0, z = 0;
//
//    for (int theta=0; theta<=360;theta++) {
//        thetaRadians = theta * M_PI / 180;
//        glBegin(GL_QUAD_STRIP);
//            for(int phi=0; phi <= 180; phi++) {
//                deltaRadians = phi * M_PI / 180;
//
//                x = radius * sin(thetaRadians) * cos(deltaRadians);
//                y = radius * sin(deltaRadians);
//                z = radius * sin(thetaRadians) * cos(deltaRadians);
//                glVertex3f(x, y, z);
//
//           }
//            glEnd();
//    }
//}

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
    float x, y, z;
    float halfLength = length * 0.5;
    glBegin(GL_QUADS);
    //for(int faces; faces <= 6; faces++) {

    // for(int vertex; vertex <= 4; vertex++) {
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
