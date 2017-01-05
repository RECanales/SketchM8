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

struct Sets
{

	std::vector <Shape> tre;
	std::vector <Shape> snowman;

	//Holiday tre shapes
	Shape treeBody; //triangle (tree-angle)
	Shape star; //pentagon
	Shape ornament1; //hexagon
	Shape ornament2; //hexagon
	Shape trunk; //rectangle

	std::vector <cv::Point> treeBodyPts =
	{
		cv::Point(460, 500),
		cv::Point(620, 500),
		cv::Point(540, 100)
	};

	std::vector <cv::Point> starPts =
	{
		cv::Point(535, 100),
		cv::Point(525, 85),
		cv::Point(540, 70),
		cv::Point(555, 85),
		cv::Point(545, 100)

	};

	std::vector <cv::Point> ornament1Pts = //top ornament
	{
		cv::Point(548, 260),
		cv::Point(543, 255),
		cv::Point(548, 250),
		cv::Point(557, 250),
		cv::Point(562, 255),
		cv::Point(557, 260)
	};

	std::vector <cv::Point> ornament2Pts = //bottom ornament
	{
		cv::Point(503, 460), //500
		cv::Point(498, 455),
		cv::Point(503, 450),
		cv::Point(512, 450),
		cv::Point(517, 455),
		//cv::Point(512, 460)
	};

	std::vector <cv::Point> trunkPts =
	{
		cv::Point(530, 500),
		cv::Point(550, 500),
		cv::Point(550, 550),
		cv::Point(530, 550)
	};


	//Snowman shapes
	Shape leftEye; //pentagon
	Shape rightEye; //pentagon
	Shape head; //hexagon
	Shape body; //hexagon
	Shape nose; //triangle
	Shape mouth; //hexagon

	std::vector <cv::Point> leftEyePts = //green pent
	{
		cv::Point(490, 140),
		cv::Point(480, 120),
		cv::Point(500, 100),
		cv::Point(520, 120),
		cv::Point(510, 140)
	};

	std::vector <cv::Point> rightEyePts = //green pent
	{
		cv::Point(570, 140),
		cv::Point(560, 120),
		cv::Point(580, 100),
		cv::Point(600, 120),
		cv::Point(590, 140)
	};

	std::vector <cv::Point> nosePts = //red tri
	{
		cv::Point(540, 170),
		cv::Point(540, 150),
		cv::Point(620, 160)
	};

	/*std::vector <cv::Point> mouthPts = //green irregular hex
	{
	cv::Point(540, 240),
	cv::Point(500, 190),
	cv::Point(510, 180),
	cv::Point(540, 230),
	cv::Point(570, 180),
	cv::Point(580, 190)

	};*/

	std::vector <cv::Point> mouthPts = //green tri
	{
		cv::Point(505,190),
		cv::Point(580,190),
		cv::Point(540, 230)
	};

	std::vector <cv::Point> headPts = //blue hex
	{
		cv::Point(470, 250),
		cv::Point(430, 150),
		cv::Point(470, 50),
		cv::Point(610, 50),
		cv::Point(650, 150),
		cv::Point(610, 250)
	};

	std::vector <cv::Point> bodyPts = //blue pent
	{
		cv::Point(470, 600),
		cv::Point(390, 425),
		cv::Point(540, 250),
		cv::Point(690, 425),
		cv::Point(610, 600)
	};

	void init_tree(int iwidth, int iheight, int bx, int by)
	{
		tre = std::vector <Shape>
		{
			Shape(trunkPts, iwidth, iheight, bx, by, 1, glm::vec3(0, 0, 1)),
			Shape(treeBodyPts, iwidth, iheight, bx, by, 0, glm::vec3(0, 1, 0)),
			Shape(starPts, iwidth, iheight, bx, by, 2, glm::vec3(0, 0, 1)),
			Shape(ornament1Pts, iwidth, iheight, bx, by, 3, glm::vec3(1, 0, 0)),
			Shape(ornament2Pts, iwidth, iheight, bx, by, 2, glm::vec3(1, 0, 0))
		};
	}

	void init_snowman(int iwidth, int iheight, int bx, int by)
	{
		snowman = std::vector <Shape>
		{
			Shape(headPts, iwidth, iheight, bx, by, 3, glm::vec3(0, 0, 1)),
			Shape(bodyPts, iwidth, iheight, bx, by, 2, glm::vec3(0, 0, 1)),
			Shape(leftEyePts, iwidth, iheight, bx, by, 2, glm::vec3(0, 1, 0)),
			Shape(rightEyePts, iwidth, iheight, bx, by, 2, glm::vec3(0, 1, 0)),
			Shape(nosePts, iwidth, iheight, bx, by, 0, glm::vec3(1, 0, 0)),
			Shape(mouthPts, iwidth, iheight, bx, by, 0, glm::vec3(0, 1, 0))
		};
	}
};