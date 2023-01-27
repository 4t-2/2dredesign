#include "../inc/Circle.hpp"
#include "../inc/macros.hpp"

Circle::Circle()
{
	type = "CIRCLE";
}

void Circle::setStart(agl::Vec<float, 2> start)
{
	this->start = start;
	xpp = start.x;
	ypp = CANVAS_Y - start.y;
}

void Circle::setRadius(double radius)
{
	this->radius = radius;

	dpfv = radius;
}

agl::Vec<float, 2> Circle::getStart()
{
	return start;
}

double Circle::getRadius()
{
	return radius;
}
