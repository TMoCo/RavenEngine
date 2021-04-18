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
	// Surface Data.
	LightSurfaceData surface;
	surface.n = normal;
	surface.p = inFrag.position;
	surface.albedo = matOut.color;
	surface.specular = clamp(matOut.specular, 0.0, 1.0);
	surface.roughness = max(matOut.roughness, 0.01);
	surface.metallic = matOut.metallic;
	
	// View Direction Vector.
	surface.v = normalize(inCommon.viewPos - surface.p);
	surface.NDotV = max(dot(surface.n, surface.v), 0.0001);
	
	// Base Reflective Index, 0.04 similar to UE4
	surface.f0 = mix(vec3(0.04), surface.albedo, surface.metallic); 

	// Forward-Lighting...
	outFinalColor.rgb = ComputeLighting(surface);
	
#if RENDER_SHADER_TYPE_TRANSLUCENT
	outFinalColor.a = matOut.alpha;
#endif
	
#endif // End of RENDER_PASS_FORWARD



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






