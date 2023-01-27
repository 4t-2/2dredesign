#pragma once

#include "Entity.hpp"
#include "../lib/AGL/agl.hpp"

class Circle : public Entity
{
	private:
		agl::Vec<float, 2> start;
		double radius;
	public:
		Circle();
		
		void setStart(agl::Vec<float, 2> start);
		void setRadius(double radius);

		agl::Vec<float, 2> getStart();
		double getRadius();
};
