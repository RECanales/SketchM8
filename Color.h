#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include "opencv2/core/opengl.hpp"
#include <vector>
#include <cmath>

class Color                  // begin declaration of the class
{
	glm::vec3 RGBcolor;
	glm::vec3 HSVcolor;
	std::vector <std::string> colors = { "red", "orange", "yellow", "green", "blue", "purple", "pink" };
	std::string c;

	public:                    // begin public section
		Color() {}
		glm::vec3 Color::getColor(cv::Mat &image, cv::Mat &hsv, cv::Point &p); //return the enum color value of the color in the image at point (x,y);
		int r, g, b;
};