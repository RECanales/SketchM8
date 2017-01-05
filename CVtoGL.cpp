#include "CVtoGL.h"

#define GLM_FORCE_RADIANS
#define PI 3.1415926536
#define INF 500000

void GLDraw::CVtoGL()
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	//use fast 4-byte alignment (default anyway) if possible
	glPixelStorei(GL_UNPACK_ALIGNMENT, (image.step & 3) ? 1 : 4);

	//set length of one complete row in data (doesn't need to equal image.cols)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, image.step / image.elemSize());

	// flip image to work with opengl bottom -> top draws
	flip(image, image, 0);
	if(flipDisplay) flip(image, image, 1);	// mirror across y axis

	// bind texture pointer to buffer then set texture parameters
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Create an image from the data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());	// rgb -> bgr for opencv

	// Draw a textured quad & frame
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
			glTexCoord2f(0, 1); glVertex3f(0, (GLfloat)iheight, 0);
			glTexCoord2f(1, 1); glVertex3f((GLfloat)iwidth, (GLfloat)iheight, 0);
			glTexCoord2f(1, 0); glVertex3f((GLfloat)iwidth, 0, 0);
		glEnd();

		glLineWidth(3);
		glColor3f(1, 0, 0.5);
		glBegin(GL_LINE_LOOP);
			glVertex2i(bx, iheight - by);
			glVertex2i(iwidth - bx, iheight - by);
			glVertex2i(iwidth - bx, by);
			glVertex2i(bx, by);
		glEnd();
		glColor4f(1, 1, 1, 0);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	// cleanup
	glDeleteTextures(1, &texture);
}

void GLDraw::symbolHandler()
{
	glm::vec3 black(0, 0, 0);
	for (unsigned i = 0; i < symbol.size(); ++i)
	{
		if (symbol[i].getColor() == black)
		{
			if (symbol[i].getType() == 2)
			{
				if (checkSet(set.tre)) {
					setFound = true;
					shapeSet = set.tre;
					glColor4f(0, 1, 0, 0.1);
					Sleep(50);
				}

				else if(checkSet(set.snowman)) {
					setFound = true;
					shapeSet = set.snowman;
					glColor4f(0, 1, 0, 0.1);
					Sleep(50);	
				}

				else
				{
					glColor4f(1, 0, 0, 0.1);
					setFound = false;
				}
			}

			
			else if (symbol[i].getType() == 1) {
				capSema = 0; draw3d = setFound = false;
			}
		}
	}

	symbol.clear();
}

bool GLDraw::animateShapes(int &animateIndex, int &setIndex, float& t, std::vector <cv::Point> &tmpPoints, std::vector <cv::Point> &shapePoints)
{
	for (int i = 0; i < shapes.size(); i++)
	{
		//if shape matches a shape within Shapeset, then animate according to the match's points
		if (shapes[i].getType() == shapeSet[setIndex].getType() && shapes[i].getColor() == shapeSet[setIndex].getColor())
		{
			if (setIndex > 0 && (shapeSet[setIndex].getColor() == shapeSet[setIndex - 1].getColor() && shapeSet[setIndex].getType() == shapeSet[setIndex - 1].getType())) 
				return false;

			shapePoints = shapes[i].original_points();
			tmpPoints = shapes[i].original_points();

			std::vector <cv::Point> shapeSetPoints = shapeSet[setIndex].original_points();
			shapeSetPoints.emplace_back(findCentroid(shapeSetPoints));

			int diffX = shapeSetPoints[animateIndex].x - shapePoints[animateIndex].x;
			int diffY = shapeSetPoints[animateIndex].y - shapePoints[animateIndex].y;

			tmpPoints[animateIndex].x = shapePoints[animateIndex].x + ((float)diffX*t);
			tmpPoints[animateIndex].y = shapePoints[animateIndex].y + ((float)diffY*t);

			Shape tmpTri(tmpPoints, iwidth, iheight, bx, by, shapeSet[setIndex].getType(), shapeSet[setIndex].getColor());
			shapes[i] = tmpTri;

			shapes[i].draw(GL_TRIANGLE_FAN);
			if (t >= 1)
			{
				t = 0;
				animateIndex = 0;
				return false;
			}
			return true;
		}
	}

	return false;
}

void GLDraw::drawShapes()
{
	float prevx = 0;
	float prevy = 0;
	glLineWidth(10);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();

	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), 0.0, 1.0);
	
	glPushMatrix();
	
	
	for (unsigned i = 0; i < shapes.size(); ++i)
		shapes[i].draw(GL_LINE_STRIP);

	static int animateIndex, setIndex;
	static float t;
	std::vector <cv::Point> tmpPoints, shapePoints;

	if (setFound)
	{
		if(animateShapes(animateIndex, setIndex, t, tmpPoints, shapePoints))
		{
			t += 0.01;
			if (animateIndex < shapeSet[setIndex].original_points().size()) animateIndex++;	
			else animateIndex = 0;
		}

		else
		{
			if (setIndex < shapeSet.size() - 1) setIndex++;
			else
			{
				if(setIndex > 0)
					setIndex--;
				else setIndex++;
			}
		}
	}
	
	glColor4f(1, 1, 1, 0);
	glPopMatrix();
	glLoadIdentity();

	if (capSema == 0) shapes.clear();
}

//checks if a single shape has a match in a given set (called by checkSet)
int GLDraw::checkShape(Shape drawnShape, std::vector <Shape> &tmp, std::vector <Shape> &compare)
{
	for (int i = 0; i < shapeSet.size(); ++i)
	{
		if (drawnShape.getType() == compare[i].getType() && drawnShape.getColor() == compare[i].getColor())
		{
			for (int j = 0; j < tmp.size(); ++j)
			{
				if (tmp[j].getType() == compare[i].getType() && tmp[j].getColor() == compare[i].getColor())
				{
					return 0;
				}
			}
			return 1;
		}
	}

	return 0;
}

//takes a set as an argument and checks it against the predefined set called shapeSet
bool GLDraw::checkSet(std::vector <Shape> &obj)
{
	if (shapes.size() != obj.size())
		return false;
	else return true;
	int e_count = 0;
	std::vector <Shape> tmp_set;

	for (unsigned i = 0; i < shapes.size(); ++i)
	{
		if (checkShape(shapes[i], tmp_set, obj) == 1)
			tmp_set.emplace_back(shapes[i]);
		else tmp_set.clear();
	}

	if (tmp_set.size() == obj.size()) return true;
	return false;
}

inline bool onSegment(cv::Point p, cv::Point q, cv::Point r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;
	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
inline int orientation(cv::Point p, cv::Point q, cv::Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The function that returns true if line segment 'p1q1' and 'p2q2' intersect.
bool doIntersect(cv::Point p1, cv::Point q1, cv::Point p2, cv::Point q2)
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

// Returns true if the point p lies inside the polygon[] with n vertices
bool isInside(std::vector <cv::Point> polygon, int n, cv::Point p)
{
	// Create a point for line segment from p to infinite
	cv::Point extreme = { INF, p.y };

	// Count intersections of the above line with sides of polygon
	int count = 0, i = 0;
	do
	{
		int next = (i + 1) % n;

		// Check if the line segment from 'p' to 'extreme' intersects with the line segment from 'polygon[i]' to 'polygon[next]'
		if (doIntersect(polygon[i], polygon[next], p, extreme))
		{
			// If the point 'p' is colinear with line segment 'i-next', then check if it lies on segment. If it lies, return true, otherwise false
			if (orientation(polygon[i], p, polygon[next]) == 0)
				return onSegment(polygon[i], p, polygon[next]);

			count++;
		}
		i = next;
	} while (i != 0);

	// Return true if count is odd, false otherwise
	return count & 1;  // Same as (count%2 == 1)
}

bool GLDraw::scanShape(std::vector <cv::Point> &points)
{
	// find ymax & ymin, max first
	std::vector <cv::Point> spoints = shape.original_points();
	if (points.size() != spoints.size()) return false;	// maybe remove
	for (unsigned i = 0; i < points.size(); ++i) 
	{
		if (isInside(points, points.size(), spoints[i])) {
			return true;
		}
	}

	return false;
}

glm::vec3 GLDraw::findColor(std::vector <cv::Point> &approx0)
{
	int radius = 15;

	int diffx = 30;
	int diffy = 30;
	glm::vec3 c;

	int blueCount, redCount, greenCount, blackCount;
	redCount = blueCount = greenCount = blackCount = 0;

	for (int i = 0; i < approx0.size(); ++i)
	{
		int cx = approx0[i].x + bx;
		int cy = approx0[i].y + by;
		//cv::circle(image, cv::Point(cx, cy), radius, cv::Scalar(0, 0, 0), 2, 8, 0);
		bool found = false;
		for (int x = cx - diffx; x < cx + diffx && x < iwidth - 1; ++x && !found)
		{
			for (int y = cy - 50; y < cy + 20 && y < iheight - 1; ++y)
			{
				if (sqrt(pow(x - cx, 2) + pow(y - cy, 2)) < radius)
				{
					c = color.getColor(image, hsvImage, cv::Point(x, y));
					if (c.x < 1 || c.y < 1 || c.z < 1)
					{
						if (c == glm::vec3(1, 0, 0)) redCount++;
						else if (c == glm::vec3(0, 1, 0)) greenCount++;
						else if (c == glm::vec3(0, 0, 1)) blueCount++;
						else blackCount++;
						found = true;
					}
				}
			}
		}
	}

	int f = std::max(redCount, blueCount);
	f = std::max(f, greenCount);
	f = std::max(f, blackCount);

	if (f == redCount) c = glm::vec3(1, 0, 0);
	else if (f == greenCount) c = glm::vec3(0, 1, 0);
	else if(f == blueCount) c = glm::vec3(0, 0, 1);
	else c = glm::vec3(0, 0, 0);
	return c;
}

bool GLDraw::findShape(int s, std::vector <cv::Point> &approx0, std::vector <Shape> &t_shapes)
{
	glm::vec3 c;
	float radius;
	int cx, cy;
	int diffx = 30;
	int diffy = 30;
	while (s < contours.size())
	{
		int prev_size = t_shapes.size();
		int i = s;
		bool cont = true;
		do
		{
			cv::approxPolyDP(cv::Mat(contours[i]), approx0, cv::arcLength(cv::Mat(contours[i]), true) * 0.05, true);

			// check if points are too close
			if (approx0.size() >= 3)
			{
				glm::vec2 p0(approx0[0].x, approx0[0].y);
				for (int j = 1; j < approx0.size(); ++j)
				{
					glm::vec2 p1(approx0[j].x, approx0[j].y);
					float point_dist = glm::length(p1 - p0);
					if (point_dist < 70) return false;
				}

				radius = 15;
				cx = approx0[1].x + bx;
				cy = approx0[1].y + by;
			}

			else return false;

			// int to represent the kind of shape detected
			int type = approx0.size() - 3;

			draw3d = true;
			if (t_shapes.size() > 1)
			{
				shape = t_shapes[t_shapes.size() - 2];
				if (scanShape(approx0));
				else
				{
					c = findColor(approx0);
					/*
					if (c == glm::vec3(0, 0, 0))
					{
						if (capSema < 2) {
							capSema = 2;
							Sleep(50);
							return false;
						}
					}
					*/
					
					approx0.emplace_back(findCentroid(approx0));
					Shape tmp(approx0, iwidth, iheight, bx, by, type, c);
					t_shapes.emplace_back(tmp);
				}
			}

			else if (t_shapes.size() == 1)
			{
				c = findColor(approx0);
				/*
				if (c == glm::vec3(0, 0, 0))
				{
					if (capSema < 2) {
						capSema = 2;
						Sleep(50);
						return false;
					}
				}
				*/
				
				approx0.emplace_back(findCentroid(approx0));
				Shape tmp(approx0, iwidth, iheight, bx, by, type, c);
				if (tmp == t_shapes[0]) t_shapes[0] = tmp;
				else
					t_shapes.emplace_back(tmp);
			}

			else
			{
				c = findColor(approx0);
				
				/*
				if (c == glm::vec3(0, 0, 0))
				{
					if (capSema < 2) {
						capSema = 2;
						Sleep(50);
						return false;
					}
				}
				*/
				approx0.emplace_back(findCentroid(approx0));
				Shape tmp(approx0, iwidth, iheight, bx, by, type, c);
				t_shapes.emplace_back(tmp);
			}

			if (t_shapes.size() > prev_size) s += 2;
			s += 2;
			cont = false;
			++i;

		} while (i < contours.size() && cont);
	}

	return false;
}

void GLDraw::outlines()
{
	cv::Mat src_gray, thresh_output;

	// center area
	cv::Mat roi = cv::Mat(image, cv::Rect(bx, by, scanWidth, scanHeight));
	
	cv::cvtColor(roi, src_gray, CV_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(2, 2));	// tiny blur
	cv::Canny(src_gray, thresh_output, 100, 300, 3);

	// apply morphological filter to help find closed edges
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2), cv::Point(1, 1));
	cv::morphologyEx(thresh_output, thresh_output, cv::MORPH_CLOSE, element);

	// find contours
	cv::Vec4f lines;
	cv::findContours(thresh_output, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	
	std::vector <cv::Point> approx0;
	approx0.resize(contours.size());
	int arrIndex = 0;
	if (capSema < 2)
	{
		arrIndex = 0;
		while (findShape(arrIndex, approx0, shapes));
		if (capSema == 1) capSema++;
	}

	if (capSema == 2 && !setFound) 
	{
		arrIndex = 0;
		while (findShape(arrIndex, approx0, symbol));
	}

	else if (capSema == 2 && setFound == true) draw3d = true;
}