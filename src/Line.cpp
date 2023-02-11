#include "../inc/Line.hpp"

void Line::drawFunction(agl::RenderWindow &window)
{
	shape.setPosition(start);
	shape.setOffset({0, 0, 2});
	shape.setSize(end - start);

	window.drawShape(shape);
}

void Line::setStart(agl::Vec<float, 2> start)
{
	this->start = start;
	xpp			= start.x;
	ypp			= CANVAS_Y - start.y;
}

void Line::setEnd(agl::Vec<float, 2> end)
{
	this->end = end;
	xop		  = end.x;
	yop		  = CANVAS_Y - end.y;
}

void Line::set(agl::Vec<float, 2> start, agl::Vec<float, 2> end)
{
	this->setStart(start);
	this->setEnd(end);
}

agl::Vec<float, 2> Line::getStart()
{
	return start;
}

agl::Vec<float, 2> Line::getEnd()
{
	return end;
}
