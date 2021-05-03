

// Input Basic Materail Paramters
layout(std140) uniform MaterialParamtersBlock
{
	vec4 Color;
	vec4 Emission;
	float Roughness;
	float Metallic;
	float Specular;
	float Alpha;
	
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
	outParams.color = inMaterial.Color * sRGBToLinearSample(ColorTexture, inData.texCoord);
	outParams.emission = inMaterial.Emission;
	outParams.roughness = inMaterial.Roughness * texture(RoughnessTexture, inData.texCoord);
	outParams.metallic = inMaterial.Metallic * texture(MetallicTexture, inData.texCoord);
	outParams.specular = inMaterial.Specular;
	outParams.alpha = inMaterial.alpha;
	
	// Default...
	vec3 normal = texture(NormalTexture, inData.texCoord)
	outParams.normal = NormalMapToWorld(normal, inData.normal, inData.tangent);
}

