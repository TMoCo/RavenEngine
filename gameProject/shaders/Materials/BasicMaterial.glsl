
// Basic Material Paramters.
layout(std140) uniform MaterialParamtersBlock
{
	vec4 color;
	vec4 emission;
	float roughness;
	float metallic;
	float specular;
}inMaterial;



//
uniform sampler2D colorTexture;



// Baisc Materail:
// 		- is just a simple pass through material.
void ComputeMaterial(in MaterialData inData, out MaterialOutput outParams)
{
	outParams.color = inMaterial.color.rgb;	
	outParams.emission = inMaterial.emission.rgb * (0.3 * abs(sin(inCommon.time * 2.0)));
	outParams.roughness = inMaterial.roughness;
	outParams.metallic = inMaterial.metallic;
	outParams.specular = inMaterial.specular;
	
	// Default...
	outParams.normal = vec3(0.0, 0.0, 1.0);
}

