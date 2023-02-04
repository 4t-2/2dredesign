#version 330 core

in vec2 UVcoord;
in vec4 fragColor;
in vec4 fragPos;
in float scale;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    color = texture(textureSampler, UVcoord) * fragColor;

	if(mod(fragPos.x+(.5*scale), 10) < (1*scale) && mod(fragPos.y+(.5*scale), 10) < (1*scale))
	{
		color = vec4(0, 0, 0, 1);
	}
}
