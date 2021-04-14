#version 450 core






// Vertex Shader Output.
in VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} inFrag;






#if RENDER_PASS_FORWARD 
// Output...
layout(location=0) out vec4 outFinalColor;
#endif




#if RENDER_PASS_DEFERRED
// Output G-Buffer...
layout(location=0) out vec4 outAlbedo;
layout(location=1) out vec3 outNormal;
layout(location=2) out vec3 outBRDF;
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
	
	
#if RENDER_SHADER_TYPE_MASKED
	// Masking...
	// TODO: Alpha Dithering for better transition.
	if (matOut.alpha < 0.5)
	{
		// discard fragment.
		discard;
	}
#endif

	
#if RENDER_PASS_FORWARD
	// Forward-Lighting...
	outFinalColor.rgb = vec3(0.0);
	
#if RENDER_SHADER_TYPE_TRANSLUCENT
	outFinalColor.a = matOut.alpha;
#endif
	
#endif // End of RENDER_PASS_FORWARD



#if RENDER_PASS_DEFERRED
	// Output G-Buffer...
	outAlbedo.rgb = matOut.color + matOut.emission;
	outAlbedo.a = length(matOut.emission) * 0.1;
	outNormal.rgb = normal;
	outBRDF.r = matOut.roughness;
	outBRDF.g = matOut.metallic;
	outBRDF.b = matOut.specular;
#endif

	
}






