#pragma once

#include "../lib/AGL/agl.hpp"
#include "Entity.hpp"

class Line : public Entity, public agl::Drawable
{
	private:
		agl::Vec<float, 2> start;
		agl::Vec<float, 2> end;

		agl::Shape *shape;

	public:
		Line(agl::Shape &lineShape);
		Line(const Line&);

		void set(agl::Vec<float, 2> start, agl::Vec<float, 2> end);

		void setStart(agl::Vec<float, 2> start);
		void setEnd(agl::Vec<float, 2> end);

		agl::Vec<float, 2> getStart();
		agl::Vec<float, 2> getEnd();
};
