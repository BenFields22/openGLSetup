#include <gl/freeglut.h>
#include <cmath>
using namespace std;

//declare global variables
//anle of rotation for the camera direction
float angle = 0.0;
//actual vector representing the cameras direction
float lx = 0.0f, lz = -1.0f;
//xz position of the camera
float x = 0.0f, z = 5.0f;

void drawSnowMan()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	//draw body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	//draw head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	//draw eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	//draw nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
}

void renderScene(void)
{
	//clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//reset transformations
	glLoadIdentity();
	//set camera
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);


	//draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	//draw 36 snowmen
	for (int i = -3; i < 3; i++)
	{
		for (int j = -3; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(i*10.0, 0, j*10.0);
			drawSnowMan();
			glPopMatrix();
		}
	}
	glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy)
{
	float fraction = 0.06f;
	int mod;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx + fraction;
		z += lz*fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx + fraction;
		z -= lz*fraction;
		break;
	}
}
void changeSize(int w, int h)
{
	//prevent a divide by zero, when window is too short
	//you cant make a window of zero width
	if (h == 0)
	{
		h = 1;
	}
	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
}
//function to execute when normal key is pressed
void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
	}

}

int main(int argc, char **argv)
{
	// init GLUT and create window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(250, 100);
	glutInitWindowSize(720, 720);
	glutCreateWindow("SNOWMAN GLOBE - GLUT Tutorial");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}