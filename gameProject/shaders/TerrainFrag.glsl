#version 330 core




// Vertex Shader Output.
in VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} fgIn;



// Uniforms...
uniform sampler2D inHeightMap;
uniform vec4 color;


// Output...
out vec4 fragColor;



// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 




void main()
{
	fragColor = vec4(fgIn.normal * 0.5 + 0.5, 1.0);
	fragColor = texture(inHeightMap, fgIn.texCoord);
	//fragColor = vec4(fgIn.texCoord, 0.0, 1.0);
}

