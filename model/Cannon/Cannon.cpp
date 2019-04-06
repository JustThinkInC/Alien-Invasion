#include <iostream>
#include <fstream>
#include <climits>
#include <math.h> 
#include <GL/freeglut.h>
#include "Cannon.h"
using namespace std;

Cannon* cannons[4];

//-- Loads mesh data in OFF format    -------------------------------------
void Cannon::loadMeshFile(const char* fname)
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

	for(int i=0; i < nvrt; i++)                         //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for(int i=0; i < ntri; i++)                         //read polygon list 
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void Cannon::normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}

//--------draws the mesh model of the cannon----------------------------
void Cannon::drawCannon()
{
	glColor3f(0.1, 0.1, 0.1);//0.4, 0.5, 0.4);
    glPushMatrix();
        glTranslatef(cannonX, cannonY, cannonZ);
        glRotatef(-90, 0, 1, 0);
        glRotatef(tilt, 0, 0, 1); // Upwards tilt
        glScalef(0.2, 0.2, 0.2);

        //Construct the object model here using triangles read from OFF file
        glBegin(GL_TRIANGLES);
            for(int tindx = 0; tindx < ntri; tindx++)
            {
               normal(tindx);
               glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
               glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
               glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
            }
        glEnd();
	glPopMatrix();

    // Cannon ball
    glPushMatrix();
        glTranslatef(cannonX, cannonY + 6 + cannonBallY, cannonZ + (length / 8)  + cannonBallX); //glTranslatef(38.88+cx, 64+cy, 0+cz);
        glColor3f(0, 0, 0);
        glScalef(0.2, 0.2, 0.2);
        glutSolidSphere(5, 36, 18);
    glPopMatrix();
}


//void display()
//{
//	float lpos[4] = {100., 100., 100., 1.0};  //light's position
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(0, cam_hgt, 200, 0, 0, 0, 0, 1, 0);
//	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
//
//    glRotatef(angle, 0.0, 1.0, 0.0);		//rotate the whole scene
//
//	drawFloor();
//
//    //drawCannon();
//
//    //--start here
//    // First bracket set
//    glPushMatrix();
//        glTranslatef(-10, 5, 17);
//        glScalef(80*0.5, 10*0.5, 6*0.5);
//        glColor3f(1, 0.64, 0);
//        glutSolidCube(1);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslatef(-20, 15, 17);//glTranslatef(-20, 25, 17);
//        glScalef(40*0.5, 30*0.5, 6*0.5);
//        glColor3f(1, 0.64, 0);
//        glutSolidCube(1);
//    glPopMatrix();
//
//    // Second bracket set
//    glPushMatrix();
//        glTranslatef(-10, 5, -17);
//        glScalef(80*0.5, 10*0.5, 6*0.5);
//        glColor3f(1, 0.64, 0);
//        glutSolidCube(1);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslatef(-20, 15, -17);//glTranslatef(-20, 25, -17);
//        glScalef(40*0.5, 30*0.5, 6*0.5);
//        glColor3f(1, 0.64, 0);
//        glutSolidCube(1);
//    glPopMatrix();
//
//    // Upwards tilted cannon...classic artillery position
//    glPushMatrix();
//      //  glTranslatef(-20, 30, 0);
//        glRotatef(30, 0, 0, 30);
//        glScalef(0.8, 0.8, 0.8);
//        //glTranslatef(20, -30, 0);
//        //drawCannon();
//    glPopMatrix();
//
//    // Cannon ball
//    glPushMatrix();
//        glTranslatef(20+cx, 40+cy, 0);//glTranslatef(38.88+cx, 64+cy, 0+cz);
//        glColor3f(0, 0, 0);
//        glutSolidSphere(5, 36, 18);
//    glPopMatrix();
//
//
//    if (smoke && cy <= -64) { // y position of cannon ball is 0 or less
//        /*glPushMatrix();
//         // Value of x when y is 0 in postive region, add offset
//        // See formula in cannon fire
//            glTranslatef(280, sy, 0);
//            glRotatef(90, 90, 0, 0);
//            glColor3f(0.8, 0.8, 0.8);
//            glutSolidTorus(4, 10, 100, 100);
//        glPopMatrix();*/
//        /**
//         * formula of circle: r^2 = (x-280)^2 + (y-0)^2 where r=10 here
//         * the y is z in this case. x is bound from 270 to 290
//         * So (y)^2 = r^2 - (x-280) ^ 2 -> y = sqrt(r^2 - (x-280)^2)
//         * Probably should use a timer managing these values.
//        **/
//        glPushMatrix();
//            glTranslatef(270, sy, 0);
//            glRotatef(90, 90, 0, 0);
//            glColor3f(0.8, 0.8, 0.8);
//            glutSolidTorus(1, 2, 100, 100);
//        glPopMatrix();
//        glPushMatrix();
//            glTranslatef(290, sy, 0);
//            glRotatef(90, 90, 0, 0);
//            glColor3f(0.8, 0.8, 0.8);
//            glutSolidTorus(1, 2, 100, 100);
//        glPopMatrix();
//        glPushMatrix();
//            glTranslatef(270, sy, 10);
//            glRotatef(90, 90, 0, 0);
//            glColor3f(0.8, 0.8, 0.8);
//            glutSolidTorus(1, 2, 100, 100);
//        glPopMatrix();
//        glPushMatrix();
//            glTranslatef(290, sy, -10);
//            glRotatef(90, 90, 0, 0);
//            glColor3f(0.8, 0.8, 0.8);
//            glutSolidTorus(1, 2, 100, 100);
//        glPopMatrix();
//        glPushMatrix();
//            glTranslatef(290, sy, 10);
//            glRotatef(90, 90, 0, 0);
//            glColor3f(0.8, 0.8, 0.8);
//            glutSolidTorus(1, 2, 100, 100);
//        glPopMatrix();
//        glPushMatrix();
//            glTranslatef(270, sy, -10);
//            glRotatef(90, 90, 0, 0);
//            glColor3f(0.8, 0.8, 0.8);
//            glutSolidTorus(1, 2, 100, 100);
//        glPopMatrix();
//    }
//    glutSwapBuffers();
//	//glFlush();
//}


//------- Initialize OpenGL parameters -----------------------------------
//void initialize()
//{
//    //loadMeshFile("../assets/Cannon.off");				//Specify mesh file name here
//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour
////
////	glEnable(GL_LIGHTING);					//Enable OpenGL states
////	glEnable(GL_LIGHT0);
//// 	glEnable(GL_COLOR_MATERIAL);
////	glEnable(GL_DEPTH_TEST);
////	glEnable(GL_NORMALIZE);
////
////	glMatrixMode(GL_PROJECTION);
////	glLoadIdentity();
////	gluPerspective(120, 1, 50, 1000);  //The camera view volume
//}  //y =ax2 + bx + c
//y = 50; x = 50
//x = 38.88, sy = 64


//void smokeTimer(int value) {
//    if (value < 3) {
//        smoke = true;
//        sy++;
//        value++;
//        glutPostRedisplay();
//        glutTimerFunc(50, smokeTimer, value);
//    } else {
//        sy = 0;
//        smoke = false;
//        glutPostRedisplay();
//    }
//}

// This version uses a parabola
/*
void cannonFire(int value) {
    firing = true;
    float a = -0.005f;
    float b = 1;
    float c = 4;

    if(value < 50) { //5 seconds
        cy = a * (cx*cx) + b * cx + c;
        cx += 10;
        //cout <<  cx << ", " << cy << endl;
        value++;
        glutPostRedisplay();
        glutTimerFunc(50, cannonFire, value);
        if (cy <= -64) glutTimerFunc(0, smokeTimer, 0);

    } else {
        firing = false;
        cx = 0;
        cy = 0;
        cz = 0;
        glutPostRedisplay();
    }
}*/

void fireCannonAnim(int index) {
    Cannon* cannon = cannons[index];
    cannon->firing = true;

    int* value = &cannon->animValues.firingValue;

    float g = 9.81;
    float theta = (cannon->tilt * M_PI) / 180;
    float velocity = 50;
    float vertV = velocity * sin(theta) - (g * *value);
    float time = (2 * velocity * sin(theta)) / g;
    double maxHeight = (velocity * time) - (0.5 * g * time * time);
    double range = (velocity * velocity * 2 * sin(2*theta) * cos(theta)) / 9.81;   // 2vInit^2 * sin(theta)*cos(theta) / 9.81

    //y = height - 4.9 * ( x / (v * cos(theta)) * ( x / (v * cos(theta)) + tan(theta)
    if (time == 0 && *value < 10) {
        cannon->cannonBallX += velocity / 5 ;
        *value = *value + 1;
        glutPostRedisplay();
        glutTimerFunc(60, fireCannonAnim, index);
    } else if (*value < time * 10) {
        cannon->cannonBallX = velocity * *value * cos(theta);//(velocity * cos(theta)) * value - (0.5 * g * value * value);//(38.88 + velocity * value) - (0.5 * g * value * value); value * velocity;
        cannon->cannonBallY = (velocity * *value * cos(theta)) - (0.5 * g * *value * *value);//tan(theta) * cx - (g / (2 * velocity * velocity * cos(theta) * cos(theta))) * cx * cx;
        //(velocity * value * cos(theta)) - (0.5 * g * value * value);//0.5 - 4.9 * ( cx / (velocity * cos(theta))) * ( cx / (velocity * cos(theta))) + tan(theta) * cx;
        //(velocity * time) - (0.5*9.81*time*time); //note the -3 is an offset to look natural
        *value = *value + 1;
        glutPostRedisplay();
        glutTimerFunc(60, fireCannonAnim, index);
       // if (cy < 40) glutTimerFunc(0, smokeTimer, 0);
    } else {
        cannon->firing = false;
        cannon->cannonBallX = 0;
        cannon->cannonBallY = 0;
        *value = 0;
        glutPostRedisplay();
    }
}


int Cannon::getLength() {
    return this->length;
}