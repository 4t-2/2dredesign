#include "../lib/AGL/agl.hpp"
#include <vector>

class Line
{
	private:
		agl::Vec<float, 2> start;
		agl::Vec<float, 2> end;
	public:
		void set(agl::Vec<float, 2> start, agl::Vec<float, 2> end);
};

void Line::set(agl::Vec<float, 2> start, agl::Vec<float, 2> end)
{
	this->start = start;
	this->end = end;
}

void drawLine(agl::RenderWindow *window, Line *line, agl::Rectangle *lineShape)
{

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

	agl::Rectangle lineShape;
	lineShape.setTexture(&blank);
	lineShape.setColor(agl::Color::White);
	lineShape.setSize({100, 100});

	while(!event.windowClose())
	{
		event.pollWindow();

		window.clear();

		window.drawShape(lineShape);

		window.display();
	}

	window.close();
}
