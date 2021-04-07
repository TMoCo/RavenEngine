// Lighting Input
layout(std140) uniform LightingBlock
{	
	// Dir-Light
	vec4 lightDir;
	vec4 lightColor;
	float lightPower;
	
} inLighting;




// Light Ouput Data
struct LightOutput
{
	vec3 diffuse;
	vec3 specular;
};




// Compute Directinal Light.
void ComputeDirLight(vec3 normal, vec3 pos, out LightOutput outLight)
{
	// 
	vec3 v = normalize(inCommon.viewPos.xyz - pos);
	vec3 l = -inLighting.lightDir.xyz;
	vec3 h = normalize(l + v);
	float dln = dot(l, normal);
	
	
	// Diffuse, Specular Factors...
	float df = max(dln, 0.0);
	float ds = max(dot(h, normal), 0.0);
	ds = pow(ds, 32);
	
	// Smooth Specular and prevernt it for lighting back faces.
	if (dln < 0.1)
	{
		ds = ds * max(mix(dln * 10.0, 1.0, 0.0), 0.0);
	}
	
	outLight.diffuse = df * inLighting.lightPower * inLighting.lightColor.rgb;
	outLight.specular = ds * inLighting.lightPower * inLighting.lightColor.rgb;
	
}

