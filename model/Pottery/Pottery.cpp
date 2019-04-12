//
// Created by George on 10/04/2019.
//

#include "Pottery.h"
#include <GL/freeglut.h>
#include <cmath>

const int wineGlassN = 20;  // Total number of vertices on the base curve
const int wineN = 8; // Num vertices for wine

// Wineglass
float wineGX[wineGlassN] = {0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2.7, 3.2, 3.6, 3.8, 3.5, 3.5, 3, 3};
//float wineGY[wineGlassN] = {0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 7.5, 8.5, 9.4, 10.4, 11.6, 12.8, 12.8, 12, 11, 10};
float wineGY[wineGlassN] = {0, 0, 0.5, 1, 2, 3, 4, 5, 6, 7, 7.5, 8.5, 9.4, 10.4, 11.6, 12.8, 12.8, 12, 11, 10};
float wineGZ[wineGlassN] = {0, 0, 0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Wine
float wineX[wineN] = {0, 1, 2, 2.7, 3, 2, 1, 0};
float wineY[wineN] = {7, 7.5, 8.5, 9.4, 10, 10, 10, 10};
float wineZ[wineN] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};

/**
 * Calculate normals
 * @param x1
 * @param y1
 * @param z1
 * @param x2
 * @param y2
 * @param z2
 * @param x3
 * @param y3
 * @param z3
 */
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

    glNormal3f(nx, ny, nz);
}


void drawWine() {
    float wx[wineGlassN], wy[wineGlassN], wz[wineGlassN];

    glColor4f(1, 0, 0, 1);

    for (int j=0; j < 36; j++) {
        for (int i = 0; i < wineN; i++) {
            float theta = -10 * M_PI / 180;
            wx[i] = (wineX[i] * cos(theta)) + (wineZ[i] * sin(theta));
            wy[i] = wineY[i];
            wz[i] = (-wineX[i] * sin(theta)) + (wineZ[i] * cos(theta));
        }

        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < wineN; i++) {
            if (i > 0)
                normal(wx[i - 1], wy[i - 1], wz[i - 1],
                       wineX[i - 1], wineY[i - 1], wineZ[i - 1],
                       wineX[i], wineY[i], wineZ[i]);

            glVertex3f(wineX[i], wineY[i], wineZ[i]);
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        glEnd();

        for (int k = 0; k < wineN; k++) {
            wineX[k] = wx[k];
            wineY[k] = wy[k];
            wineZ[k] = wz[k];
        }
    }
}


/**
 * Draw a wine glass
 * @param fill
 */
void drawWineGlass(bool fill) {
    float wx[wineGlassN], wy[wineGlassN], wz[wineGlassN];
    glPushMatrix();
    for (int j=0; j < 36; j++) {
        for (int i = 0; i < wineGlassN; i++) {
            float theta = -10 * M_PI / 180;
            wx[i] = (wineGX[i] * cos(theta)) + (wineGZ[i] * sin(theta));
            wy[i] = wineGY[i];
            wz[i] = (-wineGX[i] * sin(theta)) + (wineGZ[i] * cos(theta));
        }

        glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < wineGlassN; i++) {
                if (i > 0)
                    normal(wx[i - 1], wy[i - 1], wz[i - 1],
                           wineGX[i - 1], wineGY[i - 1], wineGZ[i - 1],
                           wineGX[i], wineGY[i], wineGZ[i]);

                glVertex3f(wineGX[i], wineGY[i], wineGZ[i]);
                glVertex3f(wx[i], wy[i], wz[i]);
            }
        glEnd();
        for (int k = 0; k < wineGlassN; k++) {
            wineGX[k] = wx[k];
            wineGY[k] = wy[k];
            wineGZ[k] = wz[k];
        }
    }
    glPopMatrix();

    if (fill) {
        glPushMatrix();
            drawWine();
        glPopMatrix();
    }
}