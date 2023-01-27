#include "../lib/AGL/agl.hpp"
#include <fstream>
#include <string>
#include <vector>

#include "../inc/Circle.hpp"
#include "../inc/Line.hpp"
#include "../inc/macros.hpp"

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
	canvas.setSize({CANVAS_X, CANVAS_Y, 0});

	float windowScale = 0;

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
		int vertices = 30;

		float *vertexBufferData = (float *)malloc(vertices * 3 * sizeof(float));
		float *UVBufferData		= (float *)malloc(vertices * 2 * sizeof(float));

		for (int i = 0; i < vertices; i++)
		{
			float angle = (PI * 2 / vertices) * i;

			agl::Vec<float, 2> position = agl::pointOnCircle(angle);

			vertexBufferData[(i * 3) + 0] = position.x;
			vertexBufferData[(i * 3) + 1] = position.y;
			vertexBufferData[(i * 3) + 2] = 0;
		}

		for (int i = 0; i < vertices; i++)
		{
			UVBufferData[(i * 2) + 0] = (vertexBufferData[(i * 3) + 0] / 2) + 0.5;
			UVBufferData[(i * 2) + 1] = (vertexBufferData[(i * 3) + 1] / 2) + 0.5;
		}

		shape.genBuffers();
		shape.setMode(GL_LINE_LOOP);
		shape.setBufferData(vertexBufferData, UVBufferData, vertices);
	});

	lineShape.setTexture(&blank);
	lineShape.setColor(agl::Color::Black);

	std::vector<Line>	line;
	std::vector<Circle> circle;

	circle.push_back(Circle());
	circle[0].setStart({0, 0});
	circle[0].setRadius(50);

	agl::Vec<float, 2> cameraPosition;

	Listener listener1(
		[&]() {
			line.push_back(Line());
			agl::Vec<float, 2> pos =
				((event.getPointerWindowPosition() - (windowSize * .5)) * windowScale) + cameraPosition;
			pos.y = pos.y;
			line[line.size() - 1].set(pos, pos);

			return;
		},
		[&]() {
			agl::Vec<float, 2> pos =
				((event.getPointerWindowPosition() - (windowSize * .5)) * windowScale) + cameraPosition;
			pos.y = pos.y;
			line[line.size() - 1].setEnd(pos);

			return;
		},
		[&]() { return; });

	Listener listener2([&]() { return; }, [&]() { return; },
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

	Listener listener3([&]() { mouseStart = event.getPointerWindowPosition(); },
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

		for (int i = 0; i < line.size(); i++)
		{
			window.drawShape(lineShape, [&](agl::RenderWindow &window, agl::Shape &shape) {
				agl::Vec<float, 2> start = line[i].getStart();
				start.y					 = start.y;
				agl::Vec<float, 2> end	 = line[i].getEnd();
				end.y					 = end.y;

				shape.setPosition(start);

				shape.setSize(end - start);

				window.drawShape(shape);
			});
		}

		for (int i = 0; i < circle.size(); i++)
		{
			window.drawShape(circleShape, [&](agl::RenderWindow &window, agl::Shape &shape) {
				shape.setPosition(circle[i].getStart());

				agl::Vec<float, 2> size = agl::Vec<float, 2>{1, 1} * circle[i].getRadius();

				shape.setOffset(size * -.5);
				shape.setSize(size);

				window.drawShape(shape);
			});
		}

		window.drawShape(canvas);

		window.display();

		listener1.update(event.isPointerButtonPressed(Button1Mask));
		listener2.update(event.isKeyPressed(XK_Return));

		if (event.isKeyPressed(XK_Up))
		{
			windowScale -= SCALEDELTA;
		}
		if (event.isKeyPressed(XK_Down))
		{
			windowScale += SCALEDELTA;
		}

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

		window.setViewport(0, 0, windowSize.x, windowSize.y);

		camera.setView({cameraPosition.x, cameraPosition.y, 10}, cameraPosition, {0, 1, 0});
		camera.setOrthographicProjection(-(windowSize.x / 2.) * windowScale, (windowSize.x / 2.) * windowScale,
										 (windowSize.y / 2.) * windowScale, -(windowSize.y / 2.) * windowScale, 0.1,
										 100);

		window.updateMvp(camera);
	}

	window.close();
}
