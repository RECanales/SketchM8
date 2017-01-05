#include "Color.h"

glm::vec3 find_nearest_color(glm::vec3 &HSV)
{
	std::vector <glm::vec3> palette =
	{
		glm::vec3(0, 255, 255),		// red
		glm::vec3(360, 255, 255),	// red
		glm::vec3(120, 255, 255),	// green
		glm::vec3(240, 255, 255)	// blue
	};

	glm::vec3 old_color = glm::vec3(HSV.x * 2, HSV.y, HSV.z);
	glm::vec3 color = palette[0];

	int c = 0;
	for (int i = 0; i < palette.size(); ++i)
	{
		float diff_a = fabs(palette[i][0] - old_color[0]);
		float diff_b = fabs(color[0] - old_color[0]);
		if (diff_a < diff_b)
		{
			color = palette[i];
			c = i;
		}
	}

	switch (c)
	{
		case 0:
		case 1:
			color = glm::vec3(1, 0, 0);
			break;
		case 2: 
			color = glm::vec3(0, 1, 0);
			break;
		case 3: 
			color = glm::vec3(0, 0, 1);
			break;
	}

	return color;
}

glm::vec3 Color::getColor(cv::Mat &image, cv::Mat &hsv, cv::Point &p)
{
	cv::Vec3b intensity = image.at<cv::Vec3b>(p.y, p.x);
	HSVcolor = glm::vec3(hsv.at<cv::Vec3b>(p.y, p.x)[0], hsv.at<cv::Vec3b>(p.y, p.x)[1], hsv.at<cv::Vec3b>(p.y, p.x)[2]);

	if (HSVcolor[1] < 15 && HSVcolor[2] > 220 && intensity[0] >= 150 && intensity[1] >= 150 && intensity[2] >= 150)
	{
		RGBcolor = glm::vec3(1, 1, 1);
		return RGBcolor;
	}

	//if(HSVcolor[2] < 160) RGBcolor = glm::vec3(0, 0, 0);	// check for black
	RGBcolor = find_nearest_color(HSVcolor);

	return RGBcolor;
}