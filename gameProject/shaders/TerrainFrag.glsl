#version 330 core




// Vertex Shader Output.
in VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} fgIn;



// Uniforms...
uniform sampler2D heightMap;
uniform vec4 color;


// Output...
out vec4 fragColor;



// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 




void main()
{
	fragColor = vec4(texture(heightMap, fgIn.texCoord).r);
}

