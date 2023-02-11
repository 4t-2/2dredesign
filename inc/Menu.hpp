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

#define BUTTON_BORDERSIZE 2
#define BUTTON_PADDING	  MENU_PADDING

#define BUTTON_UNFOCUSCOLOR \
	{                       \
		32, 30, 33          \
	}
#define BUTTON_FOCUSCOLOR \
	{                     \
		82, 82, 94        \
	}

bool pointInBox(agl::Vec<float, 2> point, agl::Vec<float, 2> size, agl::Vec<float, 2> position)
{

	if (point.x >= position.x && point.x <= position.x + size.x)
	{
		if (point.y >= position.y && point.y <= position.y + size.y)
		{
			return true;
		}
	}

	return false;
}

class Button : public agl::Drawable
{
	private:
		agl::Text		   text;
		bool			   state = false;
		bool			   hover = false;
		agl::Rectangle	   bodyShape;
		agl::Rectangle	   shadowShape;
		agl::Vec<float, 3> position;
		agl::Vec<float, 2> size;

	public:
		void clear()
		{
			text.clearText();
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
			shadowShape.setColor(BUTTON_UNFOCUSCOLOR);

			bodyShape.setRotation({0, 0, 0});
			bodyShape.setPosition(position + agl::Vec<float, 3>{BUTTON_BORDERSIZE, BUTTON_BORDERSIZE, 0});
			bodyShape.setOffset({0, 0, 0.1});
			bodyShape.setSize(size - agl::Vec<float, 2>{BUTTON_BORDERSIZE * 2, BUTTON_BORDERSIZE * 2});
			bodyShape.setTexture(texture);
			bodyShape.setColor(BUTTON_UNFOCUSCOLOR);

			text.setFont(font);
			text.setColor(TEXT_COLOR);
			text.setScale(1);
			text.setPosition(position + agl::Vec<float, 3>{BUTTON_PADDING, 0, 0.2});
			text.setText(str);
		}

		void drawFunction(agl::RenderWindow &window)
		{
			window.drawShape(bodyShape);
			window.drawShape(shadowShape);
			window.drawText(text);
		}

		void setState(bool state)
		{
			this->state = state;

			if (!state)
			{
				bodyShape.setColor(BUTTON_UNFOCUSCOLOR);
			}
			else
			{
				bodyShape.setColor(BUTTON_FOCUSCOLOR);
			}
		}

		void setHover(bool hover)
		{
			this->hover = hover;

			if (!hover)
			{
				shadowShape.setColor(BUTTON_UNFOCUSCOLOR);
			}
			else
			{
				shadowShape.setColor(BUTTON_FOCUSCOLOR);
			}
		}

		agl::Vec<float, 2> getSize()
		{
			return size;
		}

		agl::Vec<float, 3> getPosition()
		{
			return position;
		}

		bool getState()
		{
			return state;
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

		int		totalButtons = 2;
		Button *button;

		std::tuple<T...> elements;

		agl::Vec<float, 3> position;
		agl::Vec<float, 2> size;

	public:
		Menu()
		{
		}

		~Menu()
		{
			for (int i = 0; i < totalButtons; i++)
			{
				button[i].clear();
			}

			delete[] button;

			text.clearText();
		}

		void drawFunction(agl::RenderWindow &window)
		{
			window.drawShape(outerShadowShape);
			window.drawShape(borderShape);
			window.drawShape(bodyShape);
			window.drawShape(innerShadowShape);

			for (int i = 0; i < totalButtons; i++)
			{
				window.draw(button[i]);
			}
		}

		bool mouseInteraction(agl::Event *event)
		{
			for (int i = 0; i < totalButtons; i++)
			{
				if (pointInBox(event->getPointerWindowPosition(), button[i].getSize(), button[i].getPosition()))
				{
					if (event->isPointerButtonPressed(Button1Mask))
					{
						button[i].setState(true);
					}
					else
					{
						button[i].setState(false);
						button[i].setHover(true);
					}

					return true;
				}

				button[i].setHover(false);
				button[i].setState(false);
			}

			return false;
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

			button = new Button[totalButtons];

			button[0].setup(position + textOffset, buttonSize, texture, "LINE", font);

			textOffset.y += buttonSize.y;
			textOffset.y += MENU_PADDING;

			button[1].setup(position + textOffset, buttonSize, texture, "CIRCLE", font);

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

		bool getButtonState(int index)
		{
			return button[index].getState();
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
