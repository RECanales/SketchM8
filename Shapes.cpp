#include "Shapes.h"

// --- Shape class definitions --- //
void Shape::draw(GLint drawtype) {
	int tx_i = points[0].x;	// center x
	int ty_i = points[0].y;	// center y

	glPushMatrix();
	glTranslatef(tx_i, ty_i, 0);
	glRotatef(rot, 0, 0, 1);
	glTranslatef(-tx_i, -ty_i, 0);
	glColor3f(color.r, color.g, color.b);
	// side faces
	/*
	glBegin(GL_QUADS);
		for (int i = 0; i < points.size() - 1; ++i)
		{
			if (i < 1) glColor3f(color.r / 2, color.g / 2, color.b / 2);
			else if(i ==1) glColor3f(color.r / 1.5, color.g / 1.5, color.b / 1.5);
			else glColor3f(color.r / i, color.g / i, color.b / i);
			
			glVertex3f(points[i].x, points[i].y, points[i].z);
			glVertex3f(points[i + 1].x, points[i + 1].y, points[i + 1].z);
			glVertex3f(tpoints[i + 1].x, tpoints[i + 1].y, tpoints[i + 1].z);
			glVertex3f(tpoints[i].x, tpoints[i].y, tpoints[i].z);
		}
	glEnd();
	*/

	// bottom face
	glBegin(drawtype);
	for (int i = points.size() - 2; i >= 0; --i)
		glVertex3f(points[i].x, points[i].y, points[i].z);

	if (points.size() > 1) glVertex3f(points[points.size() - 2].x, points[points.size() - 2].y, points[points.size() - 2].z);
	glEnd();

	/*
	// top face
	glBegin(drawtype);
		for (int i = tpoints.size() - 1; i >= 0; --i)
			glVertex3f(tpoints[i].x, tpoints[i].y, tpoints[i].z);

		if (tpoints.size() > 1) glVertex3f(tpoints[tpoints.size() - 2].x, tpoints[tpoints.size() - 2].y, tpoints[tpoints.size() - 2].z);
	
	glEnd();
	*/
	glPopMatrix();
}

bool Shape::operator==(const Shape& a)
{
	if (this->points == a.points)
		return true;
	else
		return false;
}

Shape& Shape::operator=(const Shape& a)
{
	if (this == &a)
		return *this;
	this->points = a.points;
	this->tpoints = a.tpoints;
	this->iheight = a.iheight;
	this->iwidth = a.iwidth;
	this->color = a.color;
	this->shape = a.shape;
	this->type = a.type;
	this->originalPoints = a.originalPoints;
}