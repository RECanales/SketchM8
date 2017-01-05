#include <windows.h>
#include <iostream>
#include "CVtoGL.h"
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include "opencv2/core/opengl.hpp"

GLDraw *GLobj;

void release()	// delete stuff allocated on heap
{
	if (GLobj)
	{
		delete GLobj;
		printf("Deleted GLDraw object\n");
	}
}

void timer(int value)
{
	glutTimerFunc(100, timer, 0); // next timer call (10x)milliseconds later
	GLobj->rotation(1.5f);
	glutPostRedisplay();
}

void clickHandler(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		GLobj->track(true);
	}

	else GLobj->track(false);
}

void motionHandler(int x, int y)
{
	if (GLobj->track())
		GLobj->translate(x, GLobj->height()-y);

	glutPostRedisplay();
}

void handleKey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'i' :
			GLobj->inc();
			break;
		case 'c':
			GLobj->capture();
			break;
		case 'r':
			GLobj->spin();
			break;
		case 'f':
			GLobj->mirror();
			break;
		case 'q':
			glutDestroyWindow(glutGetWindow());
			exit(0);
			break;
		case 27:
			glutDestroyWindow(glutGetWindow());
			exit(0);
			break;
	}
}

void resize(int width, int height) 
{
	width = GLobj->width();
	height = GLobj->height();
	glutReshapeWindow(width, height);
}

void draw()
{
	static int c;
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	GLobj->readImage();

	if (!GLobj->Shape3d())
	{
		GLobj->outlines();
		GLobj->CVtoGL();	// show image under shapes
	}

	else
	{
		GLobj->outlines();
		GLobj->CVtoGL();
		GLobj->drawShapes();
		if(!GLobj->match() && GLobj->still() == 2) GLobj->symbolHandler();
	}
	
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char *argv[]) 
{
	int WIDTH = 1280;
	int HEIGHT = 720;
	std::cout << cv::getBuildInformation() << "\n\n";
	
	GLobj = new GLDraw(WIDTH, HEIGHT, 100, 50);

	// start up the glut utilities
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("SketchM8 3D");
	glewInit();

	glutDisplayFunc(draw);
	glutKeyboardFunc(handleKey);
	glutReshapeFunc(resize);
	glutMouseFunc(clickHandler);
	glutMotionFunc(motionHandler);
	glutTimerFunc(0, timer, 0);

	atexit(release);
	glutMainLoop();
	return 0;
}

