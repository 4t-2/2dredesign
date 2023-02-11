#pragma once

#include "../lib/AGL/agl.hpp"
#include "Entity.hpp"

class Line : public Entity, public agl::Drawable
{
	private:
		agl::Vec<float, 2> start;
		agl::Vec<float, 2> end;

		agl::Shape &shape;

		std::string type = "LINE";

	public:
		Line(agl::Shape &shape) : shape(shape){};

		void drawFunction(agl::RenderWindow &window);

		void set(agl::Vec<float, 2> start, agl::Vec<float, 2> end);

		void setStart(agl::Vec<float, 2> start);
		void setEnd(agl::Vec<float, 2> end);

		agl::Vec<float, 2> getStart();
		agl::Vec<float, 2> getEnd();
};
