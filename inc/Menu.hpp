#pragma once

#include "../lib/AGL/agl.hpp"
#include <tuple>

#define MENU_BORDER		2
#define MENU_PADDING	5
#define MENU_SHADOWSIZE 1

#define MENU_BORDERCOLOR \
	{                    \
		140, 140, 140    \
	}
#define MENU_SHADOWCOLOR \
	{                    \
		100, 100, 100    \
	}
#define MENU_BODYCOLOR \
	{                  \
		210, 210, 210  \
	}
#define TEXT_COLOR agl::Color::White

#define BUTTON_SHADOWSIZE  MENU_SHADOWSIZE
#define BUTTON_SHADOWCOLOR MENU_SHADOWCOLOR
#define BUTTON_BODYCOLOR   MENU_BORDERCOLOR
#define BUTTON_PADDING	   MENU_PADDING

#define BUTTON_UPCOLOR \
	{                  \
		32, 30, 33     \
	}
#define BUTTON_DOWNCOLOR \
	{                    \
		82, 82, 94       \
	}
#define BUTTON_DOWNFOCUSCOLOR \
	{                         \
		91, 91, 102           \
	}

class Button : public agl::Drawable
{
	private:
		agl::Text		   text;
		bool			   state = false;
		agl::Rectangle	   bodyShape;
		agl::Rectangle	   shadowShape;
		agl::Vec<float, 3> position;
		agl::Vec<float, 2> size;

	public:
		Button()
		{
			this->setDrawFunction([&](agl::RenderWindow &window) {
				window.drawShape(bodyShape);
				window.drawShape(shadowShape);
				window.drawText(text);
			});
		}

		void setup(agl::Vec<float, 3> position, agl::Vec<float, 2> size, agl::Texture *texture, std::string str,
				   agl::Font *font)
		{
			this->size	   = size;
			this->position = position;

			shadowShape.setRotation({0, 0, 0});
			shadowShape.setPosition(position);
			shadowShape.setOffset({0, 0, 0});
			shadowShape.setSize(size);
			shadowShape.setTexture(texture);
			shadowShape.setColor(BUTTON_SHADOWCOLOR);

			size.x -= BUTTON_SHADOWSIZE;
			size.y -= BUTTON_SHADOWSIZE;

			bodyShape.setRotation({0, 0, 0});
			bodyShape.setPosition(position);
			bodyShape.setOffset({0, 0, 0.1});
			bodyShape.setSize(size);
			bodyShape.setTexture(texture);
			bodyShape.setColor(BUTTON_UPCOLOR);

			position.x += BUTTON_PADDING;

			text.setFont(font);
			text.setColor(TEXT_COLOR);
			text.setScale(1);
			text.setPosition(position + agl::Vec<float, 3>{0, 0, 0.2});
			text.setText(str);
		}

		void setState(bool state)
		{
			this->state = state;

			if (!state)
			{
				shadowShape.setPosition(position);
				shadowShape.setRotation({0, 0, 0});
				bodyShape.setPosition(position);
				bodyShape.setRotation({0, 0, 0});
				bodyShape.setColor(BUTTON_UPCOLOR);
			}
			else
			{
				shadowShape.setPosition(position + size);
				shadowShape.setRotation({0, 0, 180});
				bodyShape.setPosition(position + size);
				bodyShape.setRotation({0, 0, 180});
				bodyShape.setColor(BUTTON_DOWNCOLOR);
			}
		}
};

template <typename... T> class Menu : public agl::Drawable
{
	private:
		agl::Rectangle outerShadowShape;
		agl::Rectangle borderShape;
		agl::Rectangle bodyShape;
		agl::Rectangle innerShadowShape;

		agl::Text text;

		Button button1;
		Button button2;

		std::tuple<T...> elements;

		agl::Vec<float, 3> position;
		agl::Vec<float, 2> size;

	public:
		Menu()
		{
			this->setDrawFunction([&](agl::RenderWindow &window) {
				window.drawShape(outerShadowShape);
				window.drawShape(borderShape);
				window.drawShape(bodyShape);
				window.drawShape(innerShadowShape);
				window.draw(button1);
				window.draw(button2);
			});
		}

		~Menu()
		{
			text.clearText();
		}

		void toggleButton(bool state)
		{
			button1.setState(state);
		}

		void setup(agl::Vec<float, 3> position, agl::Vec<float, 2> size, agl::Texture *texture, agl::Font *font)
		{
			this->position = position;
			this->size	   = size;

			outerShadowShape.setPosition(position);
			outerShadowShape.setOffset({0, 0, 0});
			outerShadowShape.setSize(size);
			outerShadowShape.setTexture(texture);
			outerShadowShape.setColor(MENU_SHADOWCOLOR);

			size.x -= MENU_SHADOWSIZE;
			size.y -= MENU_SHADOWSIZE;

			borderShape.setPosition(position);
			borderShape.setOffset({0, 0, 0.1});
			borderShape.setSize(size);
			borderShape.setTexture(texture);
			borderShape.setColor(MENU_BORDERCOLOR);

			size.x -= MENU_BORDER * 2;
			size.y -= MENU_BORDER * 2;

			innerShadowShape.setPosition(position);
			innerShadowShape.setOffset({MENU_BORDER, MENU_BORDER, 0.2});
			innerShadowShape.setSize(size);
			innerShadowShape.setTexture(texture);
			innerShadowShape.setColor(MENU_SHADOWCOLOR);

			size.x -= MENU_SHADOWSIZE;
			size.y -= MENU_SHADOWSIZE;

			bodyShape.setPosition(position);
			bodyShape.setOffset({MENU_BORDER + MENU_SHADOWSIZE, MENU_BORDER + MENU_SHADOWSIZE, 0.3});
			bodyShape.setSize(size);
			bodyShape.setTexture(texture);
			bodyShape.setColor(MENU_BODYCOLOR);

			agl::Vec<float, 3> textOffset = {MENU_BORDER + MENU_SHADOWSIZE + MENU_PADDING,
											 MENU_BORDER + MENU_SHADOWSIZE + MENU_PADDING, 0.4};

			agl::Vec<float, 2> buttonSize = size - textOffset;
			buttonSize.y				  = 30;

			button1.setup(position + textOffset, buttonSize, texture, "LINE", font);

			textOffset.y += buttonSize.y;
			textOffset.y += MENU_PADDING;

			button2.setup(position + textOffset, buttonSize, texture, "CIRCLE", font);

			text.setFont(font);
			text.setColor(TEXT_COLOR);
			text.setPosition(position + textOffset);
			text.setScale(1);

			return;
		}

		void setPosition(agl::Vec<float, 3> position)
		{
			this->position = position;

			outerShadowShape.setPosition(position);
			borderShape.setPosition(position);
			innerShadowShape.setPosition(position);
			bodyShape.setPosition(position);

			agl::Vec<float, 3> textOffset = {MENU_BORDER + MENU_SHADOWSIZE + MENU_PADDING,
											 MENU_BORDER + MENU_SHADOWSIZE + MENU_PADDING, 0.4};

			text.setPosition(position + textOffset);
		}

		void setText(std::string str)
		{
			text.clearText();
			text.setText(str);
		}

		void destroy()
		{
			text.clearText();
		}

		agl::Rectangle *getBorderShape()
		{
			return &borderShape;
		}

		agl::Rectangle *getBodyShape()
		{
			return &bodyShape;
		}

		agl::Rectangle *getOuterShadowShape()
		{
			return &outerShadowShape;
		}

		agl::Rectangle *getInnerShadowShape()
		{
			return &innerShadowShape;
		}

		agl::Text *getText()
		{
			return &text;
		}

		agl::Vec<float, 3> getPosition()
		{
			return position;
		}

		agl::Vec<float, 2> getSize()
		{
			return size;
		}
};
