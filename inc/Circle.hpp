#pragma once

#include "Entity.hpp"
#include "../lib/AGL/agl.hpp"

class Circle : public Entity, public agl::Drawable
{
	private:
		agl::Vec<float, 2> start;
		double radius;

		std::string type = "CIRCLE";

		agl::Shape &shape;
	public:
		Circle(agl::Shape &shape):shape(shape){};

		void drawFunction(agl::RenderWindow &window);

		void setStart(agl::Vec<float, 2> start);
		void setRadius(double radius);

		agl::Vec<float, 2> getStart();
		double getRadius();
};
