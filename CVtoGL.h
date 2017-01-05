#pragma once

#include <GL/glew.h>
#include <GL/gl.h>         //definitions for GL graphics routines
#include <GL/glu.h>        //definitions for GL input device handling
#include <gl/freeglut.h>       //definitions for the GLUT utility toolkit
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include "opencv2/core/opengl.hpp"
#include <vector>
#include <math.h>
#include <algorithm>
#include "Color.h"
#include "Shapes.h"
#include "Sets.h"

// draws image read from camera to opengl context
class GLDraw
{
	cv::Mat image, hsvImage;
	cv::VideoCapture cap;
	GLuint texture; //buf;
	//Camera camera;
	Color color;
	Shape shape;	// temporary shape for scanning
	std::vector <Shape> symbol, shapes;
	bool flipDisplay, draw3d;	// boolean to allow user to mirror image
	int iwidth, iheight, bx, by, scanWidth, scanHeight, DATA_SIZE;	// image width, height, border x, border y, size of image data
	int index, capSema; // index is index of shape in shapes vector
	bool rotate, tracking;
	Sets set;

	// set rec
	std::vector <Shape> shapeSet;	// set of shapes
	bool setFound = false;

	// for countours
	std::vector <std::vector <cv::Point> > contours;
	std::vector <cv::Vec4i> hierarchy;

	// helpers & private functions
	bool findShape(int , std::vector <cv::Point> &, std::vector <Shape> &);
	bool scanShape(std::vector <cv::Point> &points);

	//void setShapeSet();
	int checkShape(Shape drawnShape, std::vector <Shape> &tmp, std::vector <Shape> &compare);
	bool checkSet(std::vector <Shape> &obj);
	bool animateShapes(int &, int &, float&, std::vector <cv::Point> &, std::vector <cv::Point> &);

	public:
		GLDraw() {}
		GLDraw(int w, int h, int x, int y) {
			flipDisplay = draw3d = rotate = tracking = false;
			index = capSema = 0;	// capSema int variable used instead of bool to save last shapes in buffer when "capturing" them
			cap.open(0);
			if (!cap.isOpened())
			{
				std::cerr << "ERROR: Unable to open camera, program will now exit\n";
				Sleep(5000);
				exit(0);
			}

			iwidth = w;
			iheight = h;
			bx = x; 
			by = y;
			scanWidth = iwidth - 2 * bx;
			scanHeight = iheight - 2 * by;
			DATA_SIZE = iwidth * iheight;
			cap.set(CV_CAP_PROP_FRAME_WIDTH, iwidth);
			cap.set(CV_CAP_PROP_FRAME_HEIGHT, iheight);
			set.init_tree(iwidth, iheight, bx, by);
			set.init_snowman(iwidth, iheight, bx, by);

			// generate 1 texture to draw image to
			glGenTextures(1, &texture);
		}
		
		inline void readImage() 
		{
			cap.read(image);
			if (image.empty())
			{
				std::cerr << "ERROR! Blank frame grabbed\n";
				glutDestroyWindow(glutGetWindow());
			}

			// increase contrast & brighten
			float alpha = 1.5;
			int beta = 10;
			
			for (int i = 0; i < image.cols; ++i) {
				for (int j = 0; j < image.rows; ++j) {
					for (int c = 0; c < 3; ++c) {
						image.at<cv::Vec3b>(j, i)[c] = cv::saturate_cast<uchar>(alpha*(image.at<cv::Vec3b>(j, i)[c]) + beta);
					}
				}
			}
			
			if(!setFound) cv::cvtColor(image, hsvImage, CV_BGR2HSV);
		}

		void CVtoGL();

		inline void mirror() { flipDisplay = !flipDisplay; }
		inline int &width() { return image.cols; }
		inline int &height() { return image.rows; }
		void drawShapes();
		void outlines();
		inline bool Shape3d() { return draw3d; }
		bool spin() { rotate = !rotate;  return rotate; }
		inline void rotation(float angle) { if(rotate) shapes[index].rotate(angle); }
		void GLDraw::capture() { if (capSema < 2) capSema++; else capSema = 0; }	// capture shapes and store them
		int still() { return capSema; }
		void inc() { 
			if(index < shapes.size()) index++;
			else index = 0;
		}
		bool track(bool t) { if (capSema == 2) tracking = t; return tracking; }
		bool track() { return tracking; }
		void translate(int x, int y) { if (tracking) shapes[index].translate(x-scanWidth/2, y-scanHeight/2); }
		glm::vec3 findColor(std::vector <cv::Point> &);
		void symbolHandler();	// handles symbol recognition
		inline bool match() { return setFound; }
};