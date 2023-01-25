#include "../lib/AGL/agl.hpp"
#include <fstream>
#include <string>
#include <vector>

class Line
{
	private:
		agl::Vec<float, 2> start;
		agl::Vec<float, 2> end;

	public:
		void set(agl::Vec<float, 2> start, agl::Vec<float, 2> end);

		void setStart(agl::Vec<float, 2> start);
		void setEnd(agl::Vec<float, 2> end);

		agl::Vec<float, 2> getStart();
		agl::Vec<float, 2> getEnd();
};

void Line::setStart(agl::Vec<float, 2> start)
{
	this->start = start;
}

void Line::setEnd(agl::Vec<float, 2> end)
{
	this->end = end;
}

void Line::set(agl::Vec<float, 2> start, agl::Vec<float, 2> end)
{
	this->start = start;
	this->end	= end;
}

agl::Vec<float, 2> Line::getStart()
{
	return start;
}

agl::Vec<float, 2> Line::getEnd()
{
	return end;
}

class Listener
{
	private:
		std::function<void()> first;
		std::function<void()> hold;
		std::function<void()> last;
		bool				  pastState = false;

	public:
		Listener(std::function<void()> first, std::function<void()> hold, std::function<void()> last);
		void update(bool state);
};

Listener::Listener(std::function<void()> first, std::function<void()> hold, std::function<void()> last)
{
	this->first = first;
	this->hold	= hold;
	this->last	= last;
}

void Listener::update(bool state)
{
	if (state)
	{
		if (pastState)
		{
			hold();
		}
		else
		{
			first();

			pastState = true;
		}
	}
	else if (pastState)
	{
		last();
		pastState = false;
	}
}

int main()
{
	agl::RenderWindow window;
	window.setup({1920, 1080}, "2D ReDesign");
	window.setFPS(60);
	window.setClearColor(agl::Color::Black);

	agl::Event event;
	event.setWindow(window);

	agl::Shader shader;
	shader.loadFromFile("./vert.glsl", "./frag.glsl");
	window.getShaderUniforms(shader);
	shader.use();

	agl::Camera camera;
	camera.setOrthographicProjection(0, 1920, 1080, 0, 0.1, 100);
	camera.setView({0, 0, 10}, {0, 0, 0}, {0, 1, 0});
	window.updateMvp(camera);

	agl::Texture blank;
	blank.setBlank();

	agl::Rectangle canvas;
	canvas.setTexture(&blank);
	canvas.setColor(agl::Color::White);
	canvas.setPosition({0, 0, 0});
	canvas.setOffset({0, 0, -0.1});
	canvas.setSize({2000, 1000, 0});

	agl::Shape lineShape([](agl::Shape &shape) {
		float vertexBufferData[6];
		float UVBufferData[4];

		vertexBufferData[0] = 0;
		vertexBufferData[1] = 0;
		vertexBufferData[2] = 0;
		vertexBufferData[3] = 1;
		vertexBufferData[4] = 1;
		vertexBufferData[5] = 0;

		UVBufferData[0] = vertexBufferData[0];
		UVBufferData[1] = vertexBufferData[1];
		UVBufferData[2] = vertexBufferData[3];
		UVBufferData[3] = vertexBufferData[4];

		shape.genBuffers();
		shape.setMode(GL_LINES);
		shape.setBufferData(vertexBufferData, UVBufferData, 2);
	});

	lineShape.setTexture(&blank);
	lineShape.setColor(agl::Color::Black);

	std::vector<Line> line;

	agl::Vec<float, 2> cameraPosition;

	Listener listener1(
		[&]() {
			line.push_back(Line());
			agl::Vec<float, 2> pos = event.getPointerWindowPosition() + cameraPosition;
			pos.y				   = 1080 - pos.y;
			line[line.size() - 1].set(pos, pos);

			return;
		},
		[&]() {
			agl::Vec<float, 2> pos = event.getPointerWindowPosition() + cameraPosition;
			pos.y				   = 1080 - pos.y;
			line[line.size() - 1].setEnd(pos);

			return;
		},
		[&]() { return; });

	Listener listener2([&]() { return; }, [&]() { return; },
					   [&]() {
						   std::cout << "writing\n";

						   std::ofstream fs("test.dxf");

						   fs << "0\n";
						   fs << "SECTION\n";
						   fs << "2\n";
						   fs << "ENTITIES\n";

						   std::cout << "\n " << line.size() << "\n";

						   for (int i = 0; i < line.size(); i++)
						   {
							   std::cout << "start " << line[i].getStart() << '\n';
							   std::cout << "end   " << line[i].getEnd() << '\n';

							   // line
							   fs << "0\n";
							   fs << "LINE\n";
							   fs << "8\n";
							   fs << "0\n";
							   // start
							   fs << "10\n";
							   fs << std::to_string(line[i].getStart().x / 100) << "\n"; // x
							   fs << "20\n";
							   fs << std::to_string(line[i].getStart().y / 100) << "\n"; // x
							   fs << "30\n";
							   fs << "0.0\n"; // z
							   // end
							   fs << "11\n";
							   fs << std::to_string(line[i].getEnd().x / 100) << "\n"; // x
							   fs << "21\n";
							   fs << std::to_string(line[i].getEnd().y / 100) << "\n"; // x
							   fs << "31\n";
							   fs << "0.0\n"; // z
						   }

						   fs << "0\n";
						   fs << "ENDSEC\n";
						   fs << "0\n";
						   fs << "EOF\n";

						   fs.close();
						   return;
					   });

	agl::Vec<float, 2> cameraOffset;

	agl::Vec<float, 2> mouseStart;

	Listener listener3([&]() { mouseStart = event.getPointerWindowPosition(); },
					   [&]() {
						   std::cout << cameraOffset << '\n';
						   cameraPosition = cameraPosition - cameraOffset;

						   cameraOffset = mouseStart - event.getPointerWindowPosition();

						   cameraPosition = cameraPosition + cameraOffset;
					   },
					   [&]() {
						   cameraOffset = {0, 0};
					   });

	while (!event.windowClose())
	{
		event.pollWindow();
		event.pollPointer();
		event.pollKeyboard();

		window.clear();

		for (int i = 0; i < line.size(); i++)
		{
			window.drawShape(lineShape, [&](agl::RenderWindow &window, agl::Shape &shape) {
				agl::Vec<float, 2> start = line[i].getStart();
				start.y					 = 1080 - start.y;
				agl::Vec<float, 2> end	 = line[i].getEnd();
				end.y					 = 1080 - end.y;

				shape.setPosition(start);

				shape.setSize(end - start);

				window.drawShape(shape);
			});
		}

		window.drawShape(canvas);

		window.display();

		listener1.update(event.isPointerButtonPressed(Button1Mask));
		listener2.update(event.isKeyPressed(XK_Return));

		if (event.isPointerButtonPressed(Button2Mask))
		{
			listener3.update(true);
			window.setCursorShape(XC_fleur);
		}
		else
		{
			listener3.update(false);
			window.setCursorShape(XC_left_ptr);
		}

		camera.setView({cameraPosition.x, cameraPosition.y, 10}, cameraPosition, {0, 1, 0});

		window.updateMvp(camera);
	}

	window.close();
}
