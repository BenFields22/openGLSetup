#include <stdlib.h>	
#include <gl/freeglut.h>

using namespace std;

float angle = 0.0f;
float red=1.0, green=1.0, blue=1.0;

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//reset transformations
	glLoadIdentity();
	//set the camera
	gluLookAt(0.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	//the fucntion responsible for setting the color
	glColor3f(red, green, blue);

	glBegin(GL_TRIANGLES);
		glVertex3f(-2, -2, 0.0);
		glVertex3f(2, 0.0, 0.0);
		glVertex3f(0.0, 2, 0.0);

	glEnd();
	angle += 0.4f;

	glutSwapBuffers();
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
	if (key==27)
	{
		exit(0);
	}
	
}

void processSpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1:
		red = 1.0;
		green = 0.0;
		blue = 0.0;break;
	case GLUT_KEY_F2:
		red = 0.0;
		green = 1.0;
		blue = 0.0; break;
	case GLUT_KEY_F3:
		red = 0.0;
		green = 0.0;
		blue = 1.0; break;

	}
}

int main(int argc, char **argv)
{
	//inti GLUT and create window
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(520, 520);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutCreateWindow("OpenGL: HEllO WORLD!");



	//register callbacks
	glutDisplayFunc(renderScene);
	//reshape call back function
	glutReshapeFunc(changeSize);
	//idle func registration
	glutIdleFunc(renderScene);

	//keypress callbacks
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	//enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}