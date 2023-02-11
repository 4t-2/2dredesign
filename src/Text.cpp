#include "../inc/Text.hpp"

Text::Text(agl::Font &font)
{
	type = "TEXT";

	text.setFont(&font);
	text.setColor(agl::Color::Red);
	text.setScale(1);
}

Text::~Text()
{
	text.clearText();
}

void Text::drawFunction(agl::RenderWindow &window)
{
	window.drawText(text);
}

void Text::setStart(agl::Vec<float, 2> start)
{
	this->start = start;
	xop = start.x;
	yop = CANVAS_Y - start.y;
	
	text.setPosition(agl::Vec<float, 3>(start) + agl::Vec<float, 3>{0, 0, 2});
}

void Text::setHeight(float height)
{
	this->height = height;

	dpfv = height;
}

void Text::setTextStr(std::string textStr)
{
	this->textStr = textStr;
	
	str = textStr;

	text.clearText();
	text.setText(textStr);
}
