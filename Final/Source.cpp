#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "glm.h"
#include "imageloader.h"

#define PI 3.14

double eye[] = { 0, 0, 5.0 };
double center[] = { 0, 0, 0 };
double up[] = { 0, 1, 0 };

static int RightShoulder = 0, RightElbow = 0, LeftShoulder = 0, LeftElbow = 0, 
RightLeg = 0, LeftLeg = 0, RightKnee = 0, LeftKnee = 0,LeftLegside = 0, RightLegside = 0, 
fingerBase = 0, fingerUp = 0, ball=0;
int moving, startx, starty;



GLfloat space = 0;
GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */

GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position[] = { 0.5, 5.0, 0.0, 1.0 };
GLfloat lightPos1[] = { -0.5, -5.0, -2.0, 1.0 };
GLfloat light0_position[] = { 0, 0 , 2.0, 1.0 };

int poses[5][9] = { 
	{0,0,0,0,0,0,0,0,0},
	{5,0,0,0,0,0,0,-35,0},
	{10,180,0,180,0,0,-20,-20,0},
	{0,0,0,0,0,0,-15,-10,0},
	{0,0,0,0,0,0,-15,0,0} 
};

void setPoses(int frameNum) {
	ball = poses[frameNum][0];

	RightShoulder = poses[frameNum][1];
	RightElbow = poses[frameNum][2];

	LeftShoulder = poses[frameNum][3];
	LeftElbow = poses[frameNum][4];

	LeftLeg = poses[frameNum][5];
	RightLeg = poses[frameNum][6];

	RightKnee = poses[frameNum][7];
	LeftKnee = poses[frameNum][8];
}
static int f = 0;
void timer(int value)
{
	f = f % 5;
	setPoses(f);
	f++;
	glutPostRedisplay();
	glutTimerFunc(1500, timer, 0);
	
}


GLuint loadTexture(Image* image)
{
	GLuint textureId;
	glGenTextures(1, &textureId);			 //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
		0,							  //0 for now
		GL_RGB,					  //Format OpenGL uses for image
		image->width, image->height, //Width and height
		0,							  //The border of the image
		GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
									 //as unsigned numbers
		image->pixels);			  //The actual pixel data
	return textureId;						  //Returns the id of the texture
}
GLuint _textureId;
void main_menu(int value)
{
	if (value == 1)
	{
		Image* image = loadBMP("images/Panda.bmp");
		_textureId = loadTexture(image);
		delete image;
		glutPostRedisplay();
	}
	else if (value == 2)
	{

		Image* image = loadBMP("images/rocks.bmp");
		_textureId = loadTexture(image);
		delete image;
		/*glClearColor(0.0, 0.0, 0.0, 0.0);*/
		glutPostRedisplay();
	}
	else if (value == 3) {
		Image* image = loadBMP("images/magic.bmp");
		_textureId = loadTexture(image);
		delete image;
		glutPostRedisplay();
	}
	else if (value == 4) {
		Image* image = loadBMP("images/field.bmp");
		_textureId = loadTexture(image);
		delete image;
		glutPostRedisplay();
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, 1.0, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

void drawmodel(char* filename)
{
	GLMmodel* model = glmReadOBJ(filename);
	glmUnitize(model);
	glmFacetNormals(model);
	glmVertexNormals(model, 90.0);
	glmScale(model, .15);
	glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}
void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void turnLeft()
{
	double theta = -PI / 100;
	rotatePoint(up, theta, eye);
}

void turnRight()
{
	double theta = PI / 100;
	rotatePoint(up, theta, eye);
}

void moveUp()
{
	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 100, eye);
	rotatePoint(horizontal, PI / 100, up);
}

void moveDown()
{

	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 100, eye);
	rotatePoint(horizontal, -PI / 100, up);
}

void moveForward()
{

	double speed = 0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;
}

void moveBack()
{

	double speed = -0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;
}

void reset()
{
	double e[] = { 0.0, 0.0, 5.0 };
	double c[] = { 0.0, 0.0, 0.0 };
	double u[] = { 0.0, 1.0, 0.0 };
	for (int i = 0; i < 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
}

void initRendering()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glEnable(GL_NORMALIZE);
	//Enable smooth shading
	glShadeModel(GL_SMOOTH);
	/*main_menu(value);*/
	Image* image = loadBMP("images/rocks.bmp");
	_textureId = loadTexture(image);
	delete image;
	// Enable Depth buffer
	glEnable(GL_DEPTH_TEST);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glPushMatrix();

	glTranslatef(-6.0, -0.5, -2.5);
	glScalef(0.5, 0.7, 0.5);
	glRotatef(70.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	//Draw chest
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(3.0, 4.0, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	/////////////
	
	//Draw head
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glutWireSphere(0.7, 10, 10);
	///cap object///
	glPushMatrix();
	glScalef(8.0, 8.0, 8.0);
	glTranslatef(0.0, 0.05, 0.05);
	drawmodel((char*)"data/casquette.obj");
	glPopMatrix();

	glPopMatrix();
	///////////////
	
	//Draw right arm
	glPushMatrix();
	glTranslatef(2, 1.6, 0.0);
	glRotatef(-90, 0, 0, 1);
	glRotatef((GLfloat)RightShoulder, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef((GLfloat)RightElbow, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	//Draw finger flang 1
	glPushMatrix();
	glTranslatef(1.0, -0.2, 0.0);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();


	//Draw finger flang 1 
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 2 
	glPushMatrix();
	glTranslatef(1.0, -0.2, 0.16);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 3
	glPushMatrix();
	glTranslatef(1.0, -0.2, -0.16);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 4
	glPushMatrix();
	glTranslatef(1.0, -0.2, 0.32);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 5 
	glPushMatrix();
	glTranslatef(1.0, 0.0, -0.32);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//Draw left arm
	glPushMatrix();
	glTranslatef(-2, 1.6, 0.0);
	glRotatef(270, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)LeftShoulder, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(1, 0.0, 0.0);
	glRotatef((GLfloat)LeftElbow, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	//Draw finger flang 1
	glPushMatrix();
	glTranslatef(1.0, -0.2, 0.0);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();


	//Draw finger flang 1 
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 2 
	glPushMatrix();
	glTranslatef(1.0, -0.2, 0.16);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 3
	glPushMatrix();
	glTranslatef(1.0, -0.2, -0.16);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 4
	glPushMatrix();
	glTranslatef(1.0, -0.2, 0.32);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang 5 
	glPushMatrix();
	glTranslatef(1.0, 0.0, -0.32);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//Draw left leg
	glPushMatrix();
	glTranslatef(-0.8, -2, 0.0);
	glScalef(2.1, 0.8, 1);
	glRotatef(270, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)LeftLeg, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)LeftLegside, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(1, 0.0, 0.0);
	glRotatef((GLfloat)LeftKnee, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	//Draw left foot
	glPushMatrix();
	glTranslatef(1.3, -0.05, 0);
	glScalef(0.5, 0.5, 2);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();


	//Draw right leg
	glPushMatrix();
	glTranslatef(0.8, -2, 0.0);
	glScalef(2.1, 0.8, 1);
	glRotatef(270, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)RightLeg, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)RightLegside, 0.0, 0.0, -1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(1, 0.0, 0.0);
	glRotatef((GLfloat)RightKnee, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();


	//Draw right foot
	glPushMatrix();
	glTranslatef(1.3, 0.05, 0);
	glScalef(0.5, 0.5, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();


	glPopMatrix();


	//Draw floor
	glPushMatrix();
	glTranslatef(0, 0.0, -10.0);
	glScaled(25, 7, 10);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -1, 1);
	glTexCoord2f(2.0f, 0.0f); glVertex3f(1, -1, 1);
	glTexCoord2f(2.0f, 2.0f); glVertex3f(1, -1, -1);
	glTexCoord2f(0.0f, 2.0f); glVertex3f(-1, -1, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();


	// Objects
	glPushMatrix();
	glTranslatef(space, 0.0, 0.0);
	glTranslatef((GLfloat)ball, 0.0, 0.0);

	glScalef(3.0, 3.0, 3.0);
	glTranslatef(-1.5, -1.5, -1.0);
	drawmodel((char*)"data/soccerball.obj");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0, -2.0, -3.0);
	glScalef(15.0, 20.0, 10.0);
	glRotatef(-75.0, 0.0, 1.0, 0.0);
	drawmodel((char*)"data/football-goal.obj");
	glPopMatrix();


	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: turnLeft(); break;
	case GLUT_KEY_RIGHT: turnRight(); break;
	case GLUT_KEY_UP: moveUp(); break;
	case GLUT_KEY_DOWN: moveDown(); break;
	}
	glutPostRedisplay();
}
void Keyboard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case 's':
		if (RightShoulder >= 180) {
			break;
		}
		RightShoulder = (RightShoulder + 5) % 360;
		LeftShoulder = (LeftShoulder - 5) % 360;
		break;
	case 'S':
		if (RightShoulder <= 0) {
			break;
		}
		RightShoulder = (RightShoulder - 5) % 360;
		LeftShoulder = (LeftShoulder + 5) % 360;
		break;
	case 'e':
		if (RightElbow >= 160) {
			break;
		}
		RightElbow = (RightElbow + 5) % 360;
		LeftElbow = (LeftElbow - 5) % 360;
		break;
	case 'E':
		if (RightElbow <= 0) {
			break;
		}
		RightElbow = (RightElbow - 5) % 360;
		LeftElbow = (LeftElbow + 5) % 360;
		break;
	case 'l':
		if (RightLeg <= -90) {
			break;
		}
		RightLeg = (RightLeg - 5) % 360;
		LeftLeg = (LeftLeg - 5) % 360;
		break;
	case 'L':
		if (RightLeg >= 0) {
			break;
		}
		RightLeg = (RightLeg + 5) % 360;
		LeftLeg = (LeftLeg + 5) % 360;
		break;
	case 'k':
		if (RightKnee >= 90) {
			break;
		}
		RightKnee = (RightKnee + 5) % 360;
		LeftKnee = (LeftKnee + 5) % 360;
		break;
	case 'K':
		if (RightKnee <= 0) {
			break;
		}
		RightKnee = (RightKnee - 5) % 360;
		LeftKnee = (LeftKnee - 5) % 360;
		break;
	case 'g':
		if (RightLegside <= -90) {
			break;
		}
		RightLegside = (RightLegside - 5) % 360;
		LeftLegside = (LeftLegside - 5) % 360;
		break;
	case 'G':
		if (RightLegside >= 0) {
			break;
		}
		RightLegside = (RightLegside + 5) % 360;
		LeftLegside = (LeftLegside + 5) % 360;
		break;


	case 'f': moveForward(); break;
	case 'b': moveBack();  break;
	case 'r': reset();  break;
	case 27:  exit(0); break;
	}
	glutPostRedisplay();
}


static void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
}


static void motion(int x, int y)
{
	if (moving) {
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Model");
	initRendering();

	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, 1.0, 0.1, 10);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(Keyboard);
	glutCreateMenu(main_menu);
	glutTimerFunc(0, timer, 0);	
	glutAddMenuEntry("Panda Texture", 1);
	glutAddMenuEntry("Rocks Texture", 2);
	glutAddMenuEntry("magic Texture", 3);
	glutAddMenuEntry("field Texture", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	init();
	glutMainLoop();
	return 0;
}


