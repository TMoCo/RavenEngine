
// Basic Material Paramters.
layout(std140) uniform MaterialParamtersBlock
{
	vec4 color;
	vec4 emission;
	float roughness;
	float metallic;
	float specular;
	float alpha;
}inMaterial;




//
uniform sampler2D baseColorTexture;
uniform sampler2D AOTexture;
uniform sampler2D metallicTexture;
uniform sampler2D roughnessTexture;




// Baisc Materail:
// 		- is just a simple pass through material.
void ComputeMaterial(in MaterialData inData, out MaterialOutput outParams)
{
	vec3 v = normalize(inCommon.viewPos - inData.position);
	float NDotV =  dot(v, inData.normal);
	
	
	
	float r = texture(roughnessTexture, inData.texCoord).r;
	vec3 bc = sRGBToLinearSample(baseColorTexture, inData.texCoord).rgb * 2.0;
	float ao = texture(AOTexture, inData.texCoord).r;
	
	outParams.color = bc * ao * inMaterial.color.rgb;
	outParams.emission = inMaterial.emission.rgb;
	outParams.roughness =  r * 0.7;
	outParams.metallic =  min(1.0, (texture(metallicTexture, inData.texCoord).r) * inMaterial.metallic + 0.87);
	outParams.specular = inMaterial.specular;
	

	
	outParams.alpha = inMaterial.alpha + 0.2;
	
	// Default...
	outParams.normal = vec3(0.0, 0.0, 1.0);
}

