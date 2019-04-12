#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
#include "Cannon.h"
using namespace std;

Cannon* cannons[4];

/**
 * Set up the cannon ball
 */
void Cannon::setCannonBall() {
    float theta = (tilt * M_PI) / 180;
    cannonBallX = cannonX;
    cannonBallY = cannonY + (length * 0.2 * cos(theta)) - 2;
    cannonYOffset = cannonBallY;
    cannonBallZ = cannonZ + cos(theta) + (0.1 * length) - 2;
    cannonZOffset = cannonZ + 5;
}


/**
 * Loads mesh data in OFF format
 * @param fname
 */
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


/**
 * Function to compute the normal vector of a triangle with index tindx
 */
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


/**
 * Draw the cannon
 */
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

	// First bracket set
    glPushMatrix();
        glTranslatef(cannonX-5, 5, cannonZ);
        glRotatef(90, 0, 1, 0);
        glScalef(80*0.5, 10*0.5, 3*0.5);
        glColor3f(1, 0.64, 0);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(cannonX-5, 10, cannonZ);
        glRotatef(90, 0, 1, 0);
        glScalef(40*0.5, 15*0.5, 6*0.5);
        //glColor3f(1, 0.64, 0);
        glutSolidCube(1);
    glPopMatrix();

    // Second bracket set
    glPushMatrix();
        glTranslatef(5+cannonX, 5, cannonZ);
        glRotatef(90, 0, 1, 0);
        glScalef(80*0.5, 10*0.5, 3*0.5);
        glColor3f(1, 1, 0);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5+cannonX, 10, cannonZ);
        glRotatef(-90, 0, 1, 0);
        glScalef(40*0.5, 15*0.5, 6*0.5);
        glutSolidCube(1);
    glPopMatrix();

    // Cannon ball
    glPushMatrix();
        glTranslatef(cannonBallX, cannonBallY, cannonBallZ);
        glColor3f(1, 0, 0);
        glScalef(0.2, 0.2, 0.2);
        glutSolidSphere(5, 36, 18);
    glPopMatrix();
}


/**
 * Animation to fire cannon
 * Uses the physics formula of projectile trajectory
 * @param index
 */
void fireCannonAnim(int index) {
    Cannon* cannon = cannons[index];
    cannon->firing = true;

    int* value = &cannon->firingValue;

    float g = 9.81;
    float theta = (cannon->tilt * M_PI) / 180;
    float velocity = 35;
    float vertV = velocity * sin(theta) - (g * *value);
    float time = (2 * velocity * sin(theta)) / g;
    double maxHeight = (velocity * time) - (0.5 * g * time * time);
    double range = (velocity * velocity * 2 * sin(2*theta) * cos(theta)) / 9.81;   // 2vInit^2 * sin(theta)*cos(theta) / 9.81

    if (*value < time * 10) {
        cannon->cannonBallZ = velocity * *value * cos(theta) + cannon->cannonZOffset;
        cannon->cannonBallY = cannon->cannonYOffset + (velocity * *value * cos(theta)) - (0.5 * g * *value * *value);
        *value = *value + 1;

        glutPostRedisplay();
        glutTimerFunc(100, fireCannonAnim, index);
    } else {
        cannon->firing = false;
        cannon->setCannonBall();
        *value = 0;

        glutPostRedisplay();
    }
}


int Cannon::getLength() {
    return this->length;
}