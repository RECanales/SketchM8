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

// find shape center point
inline cv::Point findCentroid(const std::vector<cv::Point> vertices)
{
	cv::Moments mu = cv::moments(vertices);
	cv::Point centroid = cv::Point(mu.m10 / mu.m00, mu.m01 / mu.m00);

	return centroid;
}

// store all points, including raised points
class Shape
{
	int type, iheight, iwidth;
	float rot, transformx, transformy;
	std::vector <glm::vec3> points, tpoints;
	std::vector <cv::Point> originalPoints;
	std::string shape;
	glm::vec3 color;

public:
	Shape(std::vector <cv::Point> &p, int w, int h, int bx, int by, int t, glm::vec3 c) {
		type = t;
		iheight = h;
		iwidth = w;
		rot = transformx = transformy = 0;
		color = c;

		points.resize(p.size());
		tpoints.resize(p.size());

		originalPoints = p;

		// bottom points
		for (unsigned i = 0; i < p.size(); ++i)
		{
			points[i].x = p[i].x + bx;
			points[i].y = iheight - p[i].y - by;
			points[i].z = -0.1;
		}

		// top points
		for (unsigned i = 0; i < points.size(); ++i)
		{
			if(points[i].x > w/2) tpoints[i].x = points[i].x + (points[i].x - w / 2) / 3;
			else if (points[i].x < w / 2) tpoints[i].x = points[i].x - (w / 2 - points[i].x) / 3;
			else tpoints[i].x = points[i].x;

			if(points[i].y > h/2) tpoints[i].y = points[i].y + (points[i].y - h / 2) / 3;
			else if(points[i].y < h / 2) tpoints[i].y = points[i].y - (h / 2 - points[i].y) / 3;
			else tpoints[i].y = points[i].y;
			tpoints[i].z = 0;
		}
	}

	Shape() {}

	inline void setType(int newType) { type = newType; }
	inline void setColor(glm::vec3 newColor) { color = newColor; }
	inline int getType() { return type; }
	inline glm::vec3 getColor() { return color; }

	inline void rotate(float &angle) { rot += angle; }
	inline void translate(int x, int y) { transformx = x; transformy = y; }
	std::string get_shape() { return shape; }
	inline int npoints() { return points.size(); }
	inline std::vector <glm::vec3> get_points() { return points; }
	inline std::vector <cv::Point> original_points() { return originalPoints; }
	void set_color(glm::vec3 c) { color = c; }
	void draw(GLint drawtype);
	inline void clear() { points.clear(); shape.clear(); }
	Shape& operator=(const Shape &a);
	bool operator==(const Shape &a);
};
