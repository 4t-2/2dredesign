#include "../lib/AGL/agl.hpp"
#include <X11/cursorfont.h>
#include <fstream>
#include <string>
#include <vector>

#include <GL/gl.h>

#include "../inc/Circle.hpp"
#include "../inc/Line.hpp"
#include "../inc/Menu.hpp"
#include "../inc/macros.hpp"

#define CIRCLEVERTICIES 30

int roundInterval(float input, int interval)
{
	float amount = input / interval;
	amount += .5;

	return (int)amount * interval;
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

class DxfFile
{
	private:
		std::fstream *fs;

	public:
		DxfFile(std::string path);
		~DxfFile();

		void writeEntity(Entity entity);
};

DxfFile::DxfFile(std::string path)
{
	fs = new std::fstream(path, std::ios::out);

	*fs << "0\n";
	*fs << "SECTION\n";
	*fs << "2\n";
	*fs << "ENTITIES\n";
}

DxfFile::~DxfFile()
{
	*fs << "0\n";
	*fs << "ENDSEC\n";
	*fs << "0\n";
	*fs << "EOF\n";

	fs->close();

	delete fs;
}

void DxfFile::writeEntity(Entity entity)
{
	*fs << "0\n";
	*fs << entity.type << "\n";
	*fs << "8\n";
	*fs << "0\n";
	*fs << "10\n";
	*fs << std::to_string(entity.xpp) << "\n"; // x
	*fs << "20\n";
	*fs << std::to_string(entity.ypp) << "\n"; // x
	*fs << "11\n";
	*fs << std::to_string(entity.xop) << "\n"; // x
	*fs << "21\n";
	*fs << std::to_string(entity.yop) << "\n"; // x
	*fs << "40\n";
	*fs << std::to_string(entity.dpfv) << "\n"; // x
}

int main()
{
	agl::RenderWindow window;
	window.setup({1920, 1080}, "2D ReDesign");
	window.setFPS(60);
	window.setClearColor(agl::Color::Black);

	agl::Event event;
	event.setWindow(window);

	agl::Shader dotShader;
	dotShader.loadFromFile("./vert.glsl", "./dotFrag.glsl");

	agl::Shader basicShader;
	basicShader.loadFromFile("./vert.glsl", "./frag.glsl");

	agl::Camera canvasCamera;
	canvasCamera.setOrthographicProjection(0, 1920, 1080, 0, 0.1, 100);
	canvasCamera.setView({0, 0, 10}, {0, 0, 0}, {0, 1, 0});

	agl::Camera guiCamera;
	guiCamera.setOrthographicProjection(0, 1920, 1080, 0, 0.1, 100);
	guiCamera.setView({0, 0, 10}, {0, 0, 0}, {0, 1, 0});

	agl::Texture blank;
	blank.setBlank();

	agl::Font font;
	font.setup("/usr/share/fonts/TTF/Arial.TTF", 20);

	agl::Rectangle canvas;
	canvas.setTexture(&blank);
	canvas.setColor(agl::Color::White);
	canvas.setPosition({0, 0, 0});
	canvas.setOffset({0, 0, 1});
	canvas.setSize({CANVAS_X, CANVAS_Y, 0});

	Menu<int, int> testMenu;
	testMenu.setup({100, 100, 5}, {100, 200}, &blank, &font);

	float windowScale = 1;

	agl::Vec<float, 2> windowSize;

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

	agl::Shape circleShape([](agl::Shape &shape) {
		float vertexBufferData[CIRCLEVERTICIES * 3];
		float UVBufferData[CIRCLEVERTICIES * 2];

		for (int i = 0; i < CIRCLEVERTICIES; i++)
		{
			float angle = (PI * 2 / CIRCLEVERTICIES) * i;

			agl::Vec<float, 2> position = agl::pointOnCircle(angle);

			vertexBufferData[(i * 3) + 0] = position.x;
			vertexBufferData[(i * 3) + 1] = position.y;
			vertexBufferData[(i * 3) + 2] = 0;
		}

		for (int i = 0; i < CIRCLEVERTICIES; i++)
		{
			UVBufferData[(i * 2) + 0] = (vertexBufferData[(i * 3) + 0] / 2) + 0.5;
			UVBufferData[(i * 2) + 1] = (vertexBufferData[(i * 3) + 1] / 2) + 0.5;
		}

		shape.genBuffers();
		shape.setMode(GL_LINE_LOOP);
		shape.setBufferData(vertexBufferData, UVBufferData, CIRCLEVERTICIES);
	});

	circleShape.setColor(agl::Color::Black);
	circleShape.setTexture(&blank);

	lineShape.setTexture(&blank);
	lineShape.setColor(agl::Color::Black);

	std::vector<Line>	line;
	std::vector<Circle> circle;

	circle.push_back(Circle());

	agl::Vec<float, 2> cameraPosition = canvas.getSize() * .5;

	int entity = 0;

	Listener entitySwitcher(
		[&] {
			entity++;
			if (entity > 1)
			{
				entity = 0;
			}
		},
		[&] {}, [&] {});

	Listener entityDrawer(
		[&]() {
			agl::Vec<float, 2> pos =
				((event.getPointerWindowPosition() - (windowSize * .5)) * windowScale) + cameraPosition;

			pos.x = roundInterval(pos.x, 10);
			pos.y = roundInterval(pos.y, 10);

			if (entity == 0)
			{
				line.push_back(Line());
				line[line.size() - 1].set(pos, pos);
			}
			else
			{
				circle.push_back(Circle());
				circle[circle.size() - 1].setStart(pos);
			}

			return;
		},
		[&]() {
			agl::Vec<float, 2> pos =
				((event.getPointerWindowPosition() - (windowSize * .5)) * windowScale) + cameraPosition;

			pos.x = roundInterval(pos.x, 10);
			pos.y = roundInterval(pos.y, 10);

			if (entity == 0)
			{
				line[line.size() - 1].setEnd(pos);
			}
			else
			{
				circle[circle.size() - 1].setRadius((pos - circle[circle.size() - 1].getStart()).length());
			}

			return;
		},
		[&]() { return; });

	Listener dxfSaver([&]() { return; }, [&]() { return; },
					  [&]() {
						  std::string path = "./test.dxf";
						  std::cout << "writing " << line.size() << " entities to " << path << "\n";

						  std::cout << '\n';

						  DxfFile file(path);

						  for (Entity entity : line)
						  {
							  file.writeEntity(entity);
						  }

						  for (Entity entity : circle)
						  {
							  file.writeEntity(entity);
						  }

						  return;
					  });

	agl::Vec<float, 2> cameraOffset;

	agl::Vec<float, 2> mouseStart;

	Listener cameraController([&]() { mouseStart = event.getPointerWindowPosition(); },
							  [&]() {
								  cameraPosition = cameraPosition - cameraOffset;

								  cameraOffset = (mouseStart - (event.getPointerWindowPosition())) * windowScale;

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

		XWindowAttributes XWinAtt = window.getWindowAttributes();

		windowSize.x = XWinAtt.width;
		windowSize.y = XWinAtt.height;

		window.clear();

		dotShader.use();
		window.getShaderUniforms(dotShader);
		window.updateMvp(canvasCamera);
		glUniform1f(dotShader.getUniformLocation("winScale"), windowScale);

		window.drawShape(canvas);
		basicShader.use();
		window.getShaderUniforms(basicShader);
		window.updateMvp(canvasCamera);

		for (int i = 0; i < line.size(); i++)
		{
			window.drawShape(lineShape, [&](agl::RenderWindow &window, agl::Shape &shape) {
				agl::Vec<float, 3> start = line[i].getStart();
				agl::Vec<float, 3> end	 = line[i].getEnd();

				shape.setPosition(start);
				shape.setOffset({0, 0, 2});

				shape.setSize(end - start);

				window.drawShape(shape);
			});
		}

		for (int i = 0; i < circle.size(); i++)
		{
			window.drawShape(circleShape, [&](agl::RenderWindow &window, agl::Shape &shape) {
				shape.setPosition(circle[i].getStart());
				shape.setOffset({0, 0, 2});

				agl::Vec<float, 2> size = agl::Vec<float, 2>{1, 1} * circle[i].getRadius();

				shape.setSize(size);

				window.drawShape(shape);
			});
		}

		window.updateMvp(guiCamera);

		window.draw(testMenu);

		window.display();

		dxfSaver.update(event.isKeyPressed(XK_Return));

		if (event.isKeyPressed(XK_Up))
		{
			windowScale -= SCALEDELTA * windowScale;
		}
		if (event.isKeyPressed(XK_Down))
		{
			windowScale += SCALEDELTA * windowScale;
		}

		int shape = 0;

		if (event.isPointerButtonPressed(Button2Mask))
		{
			cameraController.update(true);
			shape = 58;
		}
		else
		{
			cameraController.update(false);
			shape = XC_left_ptr;
		}

		if (testMenu.mouseInteraction(&event))
		{
			shape = 60;
		}
		else
		{
			entityDrawer.update(event.isPointerButtonPressed(Button1Mask));
		}

		if (testMenu.getButtonState(0))
		{
			entity = 0;
		}
		if (testMenu.getButtonState(1))
		{
			entity = 1;
		}

		window.setCursorShape(shape);

		entitySwitcher.update(event.isKeyPressed(XK_space));

		if (event.isKeyPressed(XK_Escape))
		{
			line.clear();
			circle.clear();
		}

		window.setViewport(0, 0, windowSize.x, windowSize.y);

		canvasCamera.setView({cameraPosition.x, cameraPosition.y, 10}, cameraPosition, {0, 1, 0});
		canvasCamera.setOrthographicProjection(-(windowSize.x / 2.) * windowScale, (windowSize.x / 2.) * windowScale,
											   (windowSize.y / 2.) * windowScale, -(windowSize.y / 2.) * windowScale,
											   0.1, 10);
		guiCamera.setOrthographicProjection(0, windowSize.x, windowSize.y, 0, 0.1, 10);
	}

	blank.deleteTexture();
	font.deleteFont();
	dotShader.deleteProgram();
	basicShader.deleteProgram();

	window.close();
}
