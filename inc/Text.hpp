#pragma once

#include "../lib/AGL/agl.hpp"
#include "Entity.hpp"

class Text : public Entity, public agl::Drawable
{
	private:
		agl::Vec<float, 2> start;
		float height;
		std::string textStr;

		agl::Text text;
	public:
		Text(agl::Font &font);
		~Text();

		void setStart(agl::Vec<float, 2> start);

		void setHeight(float height);

		void setTextStr(std::string textStr);
};
