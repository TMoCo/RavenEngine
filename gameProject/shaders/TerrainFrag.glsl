#version 330 core



in vec2 texCoord;
uniform vec4 color;

out vec4 outFragColor;
uniform sampler2D heightMap;


void main()
{
	outFragColor = vec4(texture(heightMap, texCoord).r);
}

