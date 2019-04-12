/**
 * @author George Khella
 * @date March 2019
 * @brief Implementation of a castle with an open/close gate
 */

#include "Castle.h"
#include "../Shapes/Shapes.h"
#include "../../controller/loadTGA.h"
#include <math.h>
#include <GL/freeglut.h>
using namespace std;

Castle::Wall* Castle::walls[5];
Castle::Gate Castle::gate;

/**
 * Create a castle of given side-length
 * and given wall height
 * @param length
 * @param height
 */
Castle::Castle(int length, int height){
    this->length = length;
    this->height = height;
    this->yLevel = height * 0.5;

    for (int i = 0; i < 5; i++) {
        walls[i] = new Wall;
        walls[i]->length = this->length;
    }

    xFront = 5;
    yFront = yLevel;
    zFront = 0.5 * length;
}


/**
 * Load all castle textures
 */
void Castle::loadTex() {
    glGenTextures(2, texIds);

    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    loadTGA("assets/Castle/gate.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texIds[1]);
    loadTGA("assets/Castle/brick.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}


/**
 * Draw and texture the gate
 * @param length
 */
void Castle::drawGate(double length) {
    double halfLength = length * 0.5;
    glDisable(GL_LIGHTING);
   // glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    glBegin(GL_QUADS);
        glVertex3d(-halfLength, halfLength, length);    glTexCoord2f(1, 0);
        glVertex3d(halfLength, halfLength, length);     glTexCoord2f(1, 1);
        glVertex3d(halfLength, -halfLength, length);    glTexCoord2f(0, 1);
        glVertex3d(-halfLength, -halfLength, length);   glTexCoord2f(0, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    //glEnable(GL_BLEND);
}


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
void Castle::normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

    glNormal3f(nx, ny, nz);
}

/**
 * Draws a twisting tower
 * Base is either triangular or square (quad)
 * @param triangular - Whether or not the base polygon is triangular
 */
void Castle::drawPillars(bool triangular) {
    static int N;
    N = (triangular) ? 3 : 4;
    float vx[N];
    float vy[N] = {0};
    float vz[N];
    float wx[N], wy[N], wz[N];

    if (triangular) {
        vx[0] = 0; vx[1] = 5; vx[2] = 0;
        vz[0] = 0; vz[1] = 2.5; vz[2] = 5;
    } else {
        vx[0] = 0; vx[1] = 5; vx[2] = 5; vx[3] = 0;
        vz[0] = 0; vz[1] = 0; vz[2] = 5; vz[3] = 5;
    }

    for (int j=0; j < 8; j++) {
        for (int i = 0; i < N; i++) {
            float theta = -10 * M_PI / 180;
            wx[i] = (vx[i] * cos(theta)) + (vz[i] * sin(theta));
            wy[i] = vy[i]+10;
            wz[i] = (-vx[i] * sin(theta)) + (vz[i] * cos(theta));
        }

        //glBindTexture(GL_TEXTURE_2D, txId);
        if (triangular) {
            glBegin(GL_TRIANGLE_STRIP);
        } else {
            glBegin(GL_QUAD_STRIP);
        }
        for (int i = 0; i < N; i++) {
            if (i > 0)
                normal(wx[i - 1], wy[i - 1], wz[i - 1],
                       vx[i - 1], vy[i - 1], vz[i - 1],
                       vx[i], vy[i], vz[i]);

            glVertex3f(vx[i], vy[i], vz[i]);
            glVertex3f(wx[i], wy[i], wz[i]);
        }
            // Close the surface
            glVertex3f(vx[0], vy[0], vz[0]);
            glVertex3f(wx[0], wy[0], wz[0]);
        glEnd();

        for (int k = 0; k < N; k++) {
            vx[k] = wx[k];
            vy[k] = wy[k];
            vz[k] = wz[k];
        }

    }

    glPushMatrix();
        glColor4f(1, 0.84, 0, 1);
        glTranslatef(0.5*vx[N-1], vy[N-1], 2*vz[N-1]);
        drawPyramid(10);
    glPopMatrix();
}

/**
 * Draw and texture a wall
 */
void Castle::drawWall() {
    double halfLength = length * 0.5;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texIds[1]);
    glBegin(GL_QUADS);

        glVertex3d(-halfLength, halfLength, 0);         glTexCoord2f(1, 0);
        glVertex3d(halfLength, halfLength, 0);          glTexCoord2f(1, 1);
        glVertex3d(halfLength, -halfLength, 0);         glTexCoord2f(0, 1);
        glVertex3d(-halfLength, -halfLength, 0);        glTexCoord2f(0, 0);

        glVertex3d(-halfLength, halfLength, length);    glTexCoord2f(1, 0);
        glVertex3d(halfLength, halfLength, length);     glTexCoord2f(1, 1);
        glVertex3d(halfLength, -halfLength, length);    glTexCoord2f(0, 1);
        glVertex3d(-halfLength, -halfLength, length);   glTexCoord2f(0, 0);

        glVertex3d(-halfLength, halfLength, 0);         glTexCoord2f(1, 0);
        glVertex3d(-halfLength, halfLength, length);    glTexCoord2f(1, 1);
        glVertex3d(-halfLength, -halfLength, length);   glTexCoord2f(0, 1);
        glVertex3d(-halfLength, -halfLength, 0);        glTexCoord2f(0, 0);

        glVertex3d(halfLength, halfLength, 0);          glTexCoord2f(1, 0);
        glVertex3d(halfLength, halfLength, length);     glTexCoord2f(1, 1);
        glVertex3d(halfLength, -halfLength, length);    glTexCoord2f(0, 1);
        glVertex3d(halfLength, -halfLength, 0);         glTexCoord2f(0, 0);

        glVertex3d(-halfLength, halfLength, 0);         glTexCoord2f(1, 0);
        glVertex3d(-halfLength, halfLength, length);    glTexCoord2f(1, 1);
        glVertex3d(halfLength, halfLength, length);     glTexCoord2f(0, 1);
        glVertex3d(halfLength, halfLength, 0);          glTexCoord2f(0, 0);

        glVertex3d(-halfLength , -halfLength, length);  glTexCoord2f(1, 0);
        glVertex3d(-halfLength, -halfLength, 0);        glTexCoord2f(1, 1);
        glVertex3d(halfLength, -halfLength, 0);         glTexCoord2f(0, 1);
        glVertex3d(halfLength, -halfLength, length);    glTexCoord2f(0, 0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}


/**
 * Draw all the castle components
 */
void Castle::drawCastle() {

    static double gateWidth = 0.4 * height * 1.4;

    // Front wall right half
    glPushMatrix();
        glTranslated(gateWidth*1.3, yLevel, 0.5 * length);
        glScaled(0.4, ((float) height) / length, 10.0 / length);
        drawWall();
        walls[0]->x = gateWidth*1.3;
        walls[0]->y = yLevel;
        walls[0]->z = 0.5 * length;
        walls[0]->length = 0.4 * length;
    glPopMatrix();

    // Front wall left half
    glPushMatrix();
        glTranslated(-gateWidth*1.3, yLevel, 0.5*length);
        glScaled(0.4, ((float) height)/length, 10.0/length);
        drawWall();
        walls[1]->x = -gateWidth*1.4;
        walls[1]->y = yLevel;
        walls[1]->z = 0.5 * length;
        walls[1]->length = 0.4 * length;
    glPopMatrix();

    // Right wall
    glPushMatrix();
        glTranslated(0.5*length, yLevel, 0);
        glRotatef(90, 0, 1, 0);
        glScaled(1, ((float) height)/length, 10.0 / length);
        drawWall();
        walls[2]->x = 0.5 * length;
        walls[2]->y = yLevel;
        walls[2]->z = 0;
        walls[2]->length = length;
    glPopMatrix();

    // Back wall
    glPushMatrix();
        glTranslated(0, yLevel, -0.5 * length - 10);
        glScaled(1, ((float) height)/length, 10.0/length);
        drawWall();
        walls[3]->x = 0;
        walls[3]->y = yLevel;
        walls[3]->z = -0.5 * length - 10;
        walls[3]->length = length;
    glPopMatrix();

    // Left wall
    glPushMatrix();
        glTranslated(-0.5 * length, yLevel, 0);
        glRotated(90, 0, 1, 0);
        glScaled(1, ((float) height)/length, 10.0/length);
        drawWall();
        walls[4]->x = -0.5 * length;
        walls[4]->y = yLevel;
        walls[4]->z = 0;
        walls[4]->length = length;
    glPopMatrix();

    // Gate
    glPushMatrix();
        glTranslated(0, 0.75*height*1.4, 0.5 * length);
        glRotated(gate.angle, -1, 0, 0);
        glTranslated(0, -(0.75 * height * 1.4), -0.5 * length);//12);
        glTranslated(0, (0.25*height*1.4), 0.5 * length);

        glScaled(0.38, 1, 5.0 / length);
        drawGate(height * 1.4);
        gate.x = xFront / 2;
        gate.y = 40;
        gate.z = 0.5 * length;
        gate.width = gateWidth;
        gate.length = height * 1.4 * 1.25;
    glPopMatrix();

    // Front Right pillar
    glPushMatrix();
        glTranslated(0.5*length+10, 0, length * 0.5);
        glScalef(4, 4, 4);
        drawPillars(false);
    glPopMatrix();

    // Front left pillar
    glPushMatrix();
        glTranslated(-(0.5*length), 0, (length*0.5)+10);
        glRotatef(-90, 0, 1, 0);
        glScalef(4, 4, 4);
        drawPillars(true);
    glPopMatrix();

    // Rear right pillar
    glPushMatrix();
        glTranslated((0.5*length+10), 0, -(length * 0.5)-5);
        glRotatef(-90, 0, 1, 0);
        glScalef(4, 4, 4);
        drawPillars(true);
    glPopMatrix();

    // Rear left pillar
    glPushMatrix();
        glTranslated(-(0.5*length+5), 0, -(length * 0.5));
        glRotatef(-90, 0, 1, 0);
        glScalef(4, 4, 4);
        drawPillars(false);
    glPopMatrix();
}


/**
 * Animation for opening the castle gate
 * @param angle
 */
void Castle::openGateAnim(int angle) {
    if (gate.angle < 90 && !gate.open) {
        gate.angle += 5;
        gate.closed = false;
        gate.closing = false;
        gate.opening = true;
        glutPostRedisplay();
        glutTimerFunc(100, openGateAnim, gate.angle);
    } else {
        gate.opening = false;
        gate.open = true;
        glutPostRedisplay();
        return;
    }
}


/**
 * Animation for closing the gate
 * @param angle
 */
void Castle::closeGateAnim(int angle) {
    if (gate.angle > 0 && !gate.closed) {
        gate.angle -= 5;
        gate.open = false;
        gate.opening = false;
        gate.closing = true;
        glutPostRedisplay();
        glutTimerFunc(100, closeGateAnim, gate.angle);
    } else {
        gate.closed = true;
        gate.closing = false;
        glutPostRedisplay();
        return;
    }
}



int Castle::getLength() {
    return this->length;
}


int Castle::getHeight() {
    return this->height;
}
