#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <SOIL.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glslprogram.h"

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>



// the escape key:

#define ESCAPE		0x1b

int MainWindow;
GLSLProgram  *Pattern;
GLSLProgram  *Pattern2;
float Time,red = 0.0;
GLuint VBO, VAO, EBO;
GLuint texture1,texture2;
const int MS_PER_CYCLE = 16;
int change = 1;
float mix = .2f;
GLuint valid1;

void	Animate();
void	Display();
void	InitGraphics();
void	Resize(int, int);
void	Keyboard(unsigned char c, int x, int y);



// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );

	


	// setup all the graphics stuff:

	InitGraphics( );

	glutSetWindow(MainWindow);
	glutMainLoop( );

	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:

	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;
	Time = (float)ms / (float)MS_PER_CYCLE;


	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{

	glutSetWindow( MainWindow );

	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );


	// specify shading to be flat:

	glShadeModel( GL_SMOOTH );


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );

	////////////////////////////////////////////////////////////////////SET UP TEXTURES/////////////////////////////////////////////////////////////////////////////////

	glBindTexture(GL_TEXTURE_2D, texture1);

	Pattern->Use();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	Pattern->SetUniformVariable("ourTexture1", 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	Pattern->SetUniformVariable("ourTexture2", 1);

	Pattern->SetUniformVariable("mixValue", mix);
	////////////////////////////////////////////////////////////////////SET UP TEXTURES/////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////TRANFORMATION MATRICES/////////////////////////////////////////////////////////////////////////////////
	
	glm::mat4 trans;
	trans = glm::rotate(trans, (float)(glutGet(GLUT_ELAPSED_TIME)*glm::radians(0.01f)), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (float)(glutGet(GLUT_ELAPSED_TIME)*glm::radians(0.1f)), glm::vec3(0.0, 0.0, 1.0));
	
	GLfloat scaleAmount = (float)sin(glutGet(GLUT_ELAPSED_TIME)/5000.0);
	glm::mat4 trans2;
	trans2 = glm::rotate(trans2, (float)(glutGet(GLUT_ELAPSED_TIME)*glm::radians(0.01f)), glm::vec3(0.0, 0.0, 1.0));
	trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
	trans2 = glm::rotate(trans2, (float)(glutGet(GLUT_ELAPSED_TIME)*glm::radians(0.1f)), glm::vec3(0.0, 0.0, 1.0));
	trans2 = glm::scale(trans2, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
	
	GLuint transformLoc = glGetUniformLocation(valid1, "transformMat");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	////////////////////////////////////////////////////////////////////TRANFORMATION MATRICES/////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////MODELVIEWPROJECTION MATRIX/////////////////////////////////////////////////////////////////////////////////


	glm::mat4 model;
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view;
	// Note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)520 / 520, 0.1f, 100.0f);

	GLint modelLoc = glGetUniformLocation(valid1, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	GLint viewLoc = glGetUniformLocation(valid1, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projectionLoc = glGetUniformLocation(valid1, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	////////////////////////////////////////////////////////////////////MODELVIEWPROJECTION MATRIX/////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////DRAW CALLS/////////////////////////////////////////////////////////////////////////////////

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	Pattern->Use(0);

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );

	////////////////////////////////////////////////////////////////////DRAW CALLS/////////////////////////////////////////////////////////////////////////////////
}

// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( 520, 520 );

	// open the window and set its title:

	MainWindow = glutCreateWindow( "Practice" );
	glutSetWindowTitle( "Practice" );

	// set the framebuffer clear values:

	glClearColor(0.,0.,0.,0. );

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Animate);

	
	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif


	// create shader 1
	Pattern = new GLSLProgram();
	valid1 = Pattern->Create("vertex.vert", "fragment.frag");
	if (!valid1) {
		fprintf(stderr, "Shader cannot be created!\n");
	}
	else {
		fprintf(stderr, "Shader created.\n");
	}
	Pattern->SetVerbose(false);

	//create shader 2
	Pattern2 = new GLSLProgram();
	bool valid = Pattern2->Create("vertex.vert", "fragment2.frag");
	if (!valid) {
		fprintf(stderr, "Shader cannot be created!\n");
	}
	else {
		fprintf(stderr, "Shader created.\n");
	}
	Pattern2->SetVerbose(false);


	//set vertex data
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						   // Load and create a texture 
	 //GLuint texture;
	 glGenTextures(1, &texture1);
	 glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	 
											// Set the texture wrapping parameters
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 // Set texture filtering parameters
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 // Load image, create texture and generate mipmaps
	 int width, height;
	 unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	 glGenerateMipmap(GL_TEXTURE_2D);
	 SOIL_free_image_data(image);
	 glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	 glGenTextures(1, &texture2);
	 glBindTexture(GL_TEXTURE_2D, texture2);
	 // Set our texture parameters
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 // Set texture filtering
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 // Load, create texture and generate mipmaps
	 image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	 glGenerateMipmap(GL_TEXTURE_2D);
	 SOIL_free_image_data(image);
	 glBindTexture(GL_TEXTURE_2D, 0);
	

}

void
Resize(int width, int height)
{

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void
Keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	
	case ESCAPE:
		exit(0);
		break;				// happy compiler
	case 'w':
		mix += .1f;
		printf("%f mix\n", mix);
		break;
	case 's':
		mix -= .1f;
		printf("%f mix\n", mix);
		break;

	default:
		fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
	}

	// force a call to Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}