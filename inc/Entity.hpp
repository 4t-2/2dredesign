#pragma once

#include <string>

class Entity
{
	public:
		// gc 0 - entity type
		std::string type = "";
		// gc 10 - x primary point
		float xpp = 0;
		// gc 20 - y primary point
		float ypp = 0;
		// gc 11 - x other point
		float xop = 0;
		// gc 21 - y other point
		float yop = 0;
		// gc 40 - double precision floating value
		float dpfv = 0;
};
