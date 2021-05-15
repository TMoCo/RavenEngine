

// Input Basic Materail Paramters
layout(std140) uniform MaterialParamtersBlock
{
	vec4 Color;
	vec4 Emission;
	float Roughness;
	float Metallic;
	float Specular;

#if RENDER_SHADER_TYPE_TRANSLUCENT
	float Alpha;
#endif
	
} inMaterial;



// Input Textures
uniform sampler2D ColorTexture;
uniform sampler2D MetallicTexture;
uniform sampler2D RoughnessTexture;
uniform sampler2D NormalTexture;



// Baisc Materail:
// 		- is just a simple pass through material.
//
void ComputeMaterial(in MaterialData inData, out MaterialOutput outParams)
{
	outParams.color = inMaterial.Color.rgb * sRGBToLinearSample(ColorTexture, inData.texCoord).rgb;
	outParams.emission = inMaterial.Emission.rgb;
	outParams.roughness = inMaterial.Roughness * texture(RoughnessTexture, inData.texCoord).r;
	outParams.metallic = inMaterial.Metallic * texture(MetallicTexture, inData.texCoord).r;
	outParams.specular = inMaterial.Specular;
	
#if RENDER_SHADER_TYPE_TRANSLUCENT
	outParams.alpha = inMaterial.Alpha;
#else
	outParams.alpha = 1.0;
#endif
	
	// Normal Map...
	vec3 normal = SampleNormalMap(NormalTexture, inData.texCoord);
	outParams.normal = TangentToWorld(normal, inData.normal, inData.tangent);
}

