#include "../inc/Circle.hpp"

void Circle::drawFunction(agl::RenderWindow &window)
{
	shape.setPosition(start);
	shape.setOffset({0, 0, 2});
	shape.setSize(agl::Vec<float, 2>{1, 1} * radius);

	window.drawShape(shape);
}

void Circle::setStart(agl::Vec<float, 2> start)
{
	this->start = start;
	xpp			= start.x;
	ypp			= CANVAS_Y - start.y;
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
