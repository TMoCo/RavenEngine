#version 450 core






// Vertex Shader Output.
in VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} inFrag;







// Output...
out vec4 fragColor;



void main()
{
	// Normalize Input Normal
	vec3 normal = normalize(inFrag.normal);

	
	
	// Material Input...
	MaterialData matIn;
	matIn.position = inFrag.position;
	matIn.normal = normal;
	matIn.texCoord = inFrag.texCoord;
	
	// Material Compute Output.
	MaterialOutput matOut;
	
	// Compute Material Function...
	ComputeMaterial(matIn, matOut);
	
	// Lighting...
	LightOutput lightOut;
	ComputeDirLight(normal, inFrag.position, lightOut);
	

	fragColor.rgb = 
	  matOut.color * lightOut.diffuse 
	+ (1.0 - matOut.roughness) * lightOut.specular 
	+ matOut.emission;
	
	fragColor.a = 1.0;
}






