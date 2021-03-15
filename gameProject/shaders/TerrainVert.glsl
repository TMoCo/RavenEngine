#version 330 core



layout(location=0) in vec3 inPosition;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec2 scale;
uniform sampler2D heightMap;
uniform float heightFactor;


out vec2 texCoord;


void main()
{
	vec2 uv = vec2(inPosition.x / scale.x, inPosition.z / scale.y);
	float height = texture(heightMap, uv).r; 


	texCoord = uv;
	gl_Position = proj * view * model * vec4(inPosition.x, height * heightFactor, inPosition.z, 1.0);
}

