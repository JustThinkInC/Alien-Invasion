//
// Created by George on 1/04/2019.
//

#include "Spaceship.h"
#include "../Shapes/Shapes.h"
#include "../../Controller/loadTGA.h"

Spaceship::AnimValues Spaceship::animValues;

Spaceship::Spaceship(int radius) {
    this->radius = radius;
    this->boosterRadius = 0.4 * radius;
    this->bodyHeight = 3 * radius;
    animValues.takeOffValue = 0;
    animValues.grounded = true;
    animValues.flying = false;
    animValues.x = 0;
    animValues.y = 0;
    animValues.z = 0;

    this->texture = true;
}


/**
 * Load the textures to use on spaceship
 */
void Spaceship::loadTex() {
    glGenTextures(2, texIds);

    //Wing tip texture
    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    loadTGA("../assets/Spaceship/metal.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Flames texture
    glBindTexture(GL_TEXTURE_2D, texIds[1]);
    loadTGA("../assets/Spaceship/fire.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

}


void Spaceship::setRadius(double radius) {
    this->radius = radius;
}


double Spaceship::getRadius() {
    return this->radius;
}


double Spaceship::getBodyHeight() {
    return this->bodyHeight;
}

bool Spaceship::isGrounded() {
    return this->animValues.grounded;
}

bool Spaceship::isFlying() {
    return this->animValues.flying;
}

/**
 * Draw a single flame particle and texture it for booster
 */
void Spaceship::drawFlameParticle() {
    if (texture) {
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, texIds[1]);
    }
    // Use a triangle to create a nicer flame effect
    glBegin(GL_TRIANGLES);
        glVertex3f(0, 1, 0);       if(texture) glTexCoord2f(1, 0);
        glVertex3f(1, 1, 0);       if(texture) glTexCoord2f(0, 1);
        glVertex3f(0.5, 0, 0);     if(texture) glTexCoord2f(1, 1);
    glEnd();

    if (texture) {
        glTexCoord2f(0, 0);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }
}


/**
 * Draws 4 boosters for the spaceship
 * Each is positioned in the centre of a
 * quadrant of the base
 */
void Spaceship::drawBoosters() {
    //Boosters
    float xDir = 1;
    float zDir = 1;
    for(int i=0; i < 2; i++) {
        if (i % 2 == 1) {
            xDir = -1;
            zDir = -1;
        }

        glPushMatrix();
            glTranslatef(xDir * boosterRadius, animValues.y, zDir * boosterRadius);
            drawCylinder(0.3 * radius, 20);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(xDir * boosterRadius, animValues.y, -zDir * boosterRadius);
            drawCylinder(0.3 * radius, 20);
        glPopMatrix();
    }
}


/**
 * Draw a flame per booster
 * Each flame has 50 particles
 */
void Spaceship::drawFlames() {
    float xDir = 1;
    float zDir = 1;
    // Create a flame per booster
    for (int i = 0; i < 4; i++) {
        // Decide whether to switch directions
        if (i % 2 == 1) {
            xDir = -1;
            zDir = -1;
        }

        // Creat the 50 particles per booster
        for (int j = 0; j < FLAMES; j++) {
            int roundedRadius = round(boosterRadius);
            int maxY = round(animValues.y) - 5;
            int minY = round(animValues.y) - 40;

            double flameA = rand() * 2 * M_PI;  // Random angle
            double flameR = (roundedRadius * 0.75) * sqrt(rand() % roundedRadius); // Random radius for flame

            double flameX = flameR * cos(flameA); // X value based on randomised angle and radius
            double flameZ = rand() % (roundedRadius * 2); // Random z value between 0 and radius
            double flameY = rand() % (maxY - minY + 1) + minY;

            // Draw the flame in the +Z direction
            glPushMatrix();
                glTranslatef(xDir * (flameX - 5), flameY, zDir * flameZ);
                glScalef(10, 10, 10);
                drawFlameParticle();
            glPopMatrix();

            // Draw the flame in the -Z direction
            glPushMatrix();
                glTranslatef(xDir * (flameX - 5), flameY, -zDir * flameZ);
                glScalef(5, 5, 5);
                drawFlameParticle();
            glPopMatrix();
        }
    }
}


/**
 * Draw the wing tips and texture them
 */
void Spaceship::drawWingTips() {
   if (texture) {
       glDisable(GL_LIGHTING);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, texIds[0]);
   }
    glBegin(GL_TRIANGLES);
        // Right wing tip
        if(texture) {glTexCoord2f(1, 0);}    glVertex3f(radius, 10+animValues.y, 0);
        if(texture) {glTexCoord2f(1, 1);}    glVertex3f(radius, 30+animValues.y, 0);
        if(texture) {glTexCoord2f(0, 1);}    glVertex3f(radius+20, 10+animValues.y, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
        // Left wing tip
        if(texture) {glTexCoord2f(1, 0);}    glVertex3f(-radius, 10+animValues.y, 0);
        if(texture) {glTexCoord2f(1, 1);}    glVertex3f(-radius, 30+animValues.y, 0);
        if(texture) {glTexCoord2f(0, 1);}    glVertex3f(-(radius+20), 10+animValues.y, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
        // Front wing tip
        if(texture) {glTexCoord2f(1, 0);}    glVertex3f(0, 10+animValues.y,  radius);
        if(texture) {glTexCoord2f(1, 1);}    glVertex3f(0, 30+animValues.y,  radius);
        if(texture) {glTexCoord2f(0, 1);}    glVertex3f(0, 10+animValues.y, radius+20);
    glEnd();

    glBegin(GL_TRIANGLES);
        //Rear wing tip
        if(texture) {glTexCoord2f(1, 0);}     glVertex3f(0, 10+animValues.y, -radius);
        if(texture) {glTexCoord2f(1, 1);}     glVertex3f(0, 30+animValues.y, -radius);
        if(texture) {glTexCoord2f(0, 1);}     glVertex3f(0, 10+animValues.y, -(radius+20));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between right and front (SE)
        if(texture) {glTexCoord2f(1, 0);}     glVertex3f(radius*cos(45*M_PI/180), 10+animValues.y, radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(1, 1);}     glVertex3f(radius*cos(45*M_PI/180), 30+animValues.y, radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(0, 1);}     glVertex3f((radius+20)*cos(45*M_PI/180), 10+animValues.y, (radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between left and front (SW)
        if(texture) {glTexCoord2f(1, 0);}     glVertex3f(-radius*cos(45*M_PI/180), 10+animValues.y, radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(1, 1);}     glVertex3f(-radius*cos(45*M_PI/180), 30+animValues.y, radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(0, 1);}     glVertex3f(-(radius+20)*cos(45*M_PI/180), 10+animValues.y, (radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between right and back (NE)
        if(texture) {glTexCoord2f(1, 0);}     glVertex3f(radius*cos(45*M_PI/180), 10+animValues.y, -radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(1, 1);}     glVertex3f(radius*cos(45*M_PI/180), 30+animValues.y, -radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(0, 1);}     glVertex3f((radius+20)*cos(45*M_PI/180), 10+animValues.y, -(radius+20)*sin(45*M_PI/180));
    glEnd();

    glBegin(GL_TRIANGLES);
        // Wing tip between left and back (NW)
        if(texture) {glTexCoord2f(1, 0);}     glVertex3f(-radius*cos(45*M_PI/180), 10+animValues.y, -radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(1, 1);}     glVertex3f(-radius*cos(45*M_PI/180), 30+animValues.y, -radius*sin(45*M_PI/180));
        if(texture) {glTexCoord2f(0, 1);}     glVertex3f(-(radius+20)*cos(45*M_PI/180), 10+animValues.y, -(radius+20)*sin(45*M_PI/180));
    glEnd();

    if(texture) {
        glTexCoord2f(0, 0);
        glDisable(GL_TEXTURE_2D);
    }
    glEnable(GL_LIGHTING);
}


/**
 * Draw the entire spaceship
 */
void Spaceship::drawSpaceship() {
    //Body
    glPushMatrix();
        glTranslatef(0, 10+animValues.y, 0);
        if (!texture) glColor4f(0.18, 0.18, 0.18, 1);
        drawCylinder(radius, 3*radius);
    glPopMatrix();

    // Top part
    glPushMatrix();
        glTranslatef(0, radius*3+10+animValues.y, 0);
        if (!texture) glColor4f(0.18, 0.18, 0.18, 1);
        drawCone(radius, radius);
    glPopMatrix();

    //Wing tips
    glPushMatrix();
        if (!texture) glColor4f(0.18, 0.18, 0.18, 1);
        drawWingTips();
    glPopMatrix();

    // Boosters
    glPushMatrix();
        if (!texture) glColor4f(0.18, 0.18, 0.18, 1);
        drawBoosters();
    glPopMatrix();

    // Render the flames
    if (animValues.takeOffValue < FLIGHT_TIME && !animValues.grounded) {
        glPushMatrix();
            if (!texture) glColor4f(0.18, 0.18, 0.18, 1);
            drawFlames();
        glPopMatrix();
    }
}


/**
 * Animation for spaceship takeoff
 * @param value
 */
void Spaceship::takeOffAnim(int value) {
    if (value < FLIGHT_TIME) {
        animValues.grounded = false;
        animValues.y += 10;
        animValues.takeOffValue ++;
        animValues.flying = true;
        glutPostRedisplay();
        glutTimerFunc(100, takeOffAnim, animValues.takeOffValue);
    } else {
        animValues.flying = false;
    }
}