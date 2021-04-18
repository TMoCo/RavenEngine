#version 450 core




// Vertex Shader Output.
in VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} inFrag;



// Uniforms...
uniform sampler2D inHeightMap;




#if RENDER_PASS_DEFERRED
// Output G-Buffer...
layout(location=0) out vec4 outAlbedo;
layout(location=1) out vec3 outNormal;
layout(location=2) out vec4 outBRDF;
layout(location=3) out vec3 outEmission;
#endif





// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 




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
	

#if RENDER_PASS_DEFERRED
	// Output G-Buffer...
	outAlbedo.rgb = matOut.color;
	outEmission.rgb = matOut.emission;
	outNormal.rgb = normal;
	outBRDF.r = matOut.roughness;
	outBRDF.g = matOut.metallic;
	outBRDF.b = clamp(matOut.specular, 0.0, 1.0);
#endif

}

