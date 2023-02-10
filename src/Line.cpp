#include "../inc/Line.hpp"

Line::Line(agl::Shape &lineShape)
{
	type = "LINE";

	shape = &lineShape;

	std::cout << "init " << shape << '\n';

	this->setDrawFunction([&](agl::RenderWindow &window) {
		shape->setPosition(start);
		shape->setOffset({0, 0, 2});

		shape->setSize(end - start);

	std::cout << shape << '\n';
		std::cout << this << '\n';

		window.drawShape(lineShape);
	});
}

Line::Line(const Line& line)
{
	this->start = line.start;
	this->end = line.end;
	this->shape = line.shape;
	this->drawFunction = line.drawFunction;
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
