//	Lighting.glsl:
//		- shared between multiple shader types and domains to perform lighting.
//		- contain functionality to help with lighting, deferred render, etc...





#define MAX_NUM_LIGHTS 64






// Lighting Input
layout(std140) uniform LightingBlock
{
	// 
	vec4 lightsDataA[MAX_NUM_LIGHTS];
	
	// 
	vec4 lightsDataB[MAX_NUM_LIGHTS];
	
	// 
	vec4 lightsDataC[MAX_NUM_LIGHTS];
	
	// The start index in the light arrays for point light data. 
	int pointLightStartIndex;
	
	// The start index in the light arrays for spot light data.  
	int spotLightStartIndex;
	
	// The current number of lights in the light blocks numLights <= MAX_NUM_LIGHTS.
	int numLights;
	
} inLighting;







// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 


void GetDirLight(int idx, out vec3 lightDir, out vec3 lightPower)
{
	lightDir = inLighting.lightsDataA[idx].xyz;
	lightPower = vec4(inLighting.lightsDataA[idx].w, inLighting.lightsDataB[idx].xy);
}


void GetPointLight(int idx, out vec3 lightPos, out vec3 lightPower)
{
	lightPos = inLighting.lightsDataA[idx].xyz;
	lightPower = vec4(inLighting.lightsDataA[idx].w, inLighting.lightsDataB[idx].xy);
}


void GetPointLight(int idx, out vec3 lightPos, out vec3 lightDir, out vec3 lightPower, out float innerRadius, out float outerRadius)
{
	lightPos = inLighting.lightsDataA[idx].xyz;
	lightDir = vec4(inLighting.lightsDataA[idx].w, inLighting.lightsDataB[idx].xy);
	lightPower = vec4(inLighting.lightsDataA[idx].zw, inLighting.lightsDataC[idx].x);
	innerRadius = inLighting.lightsDataC[idx].y;
	outerRadius = inLighting.lightsDataC[idx].z;
}


// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 





// Compute Directinal Light.
void ComputeDirLight(vec3 normal, vec3 pos, out LightOutput outLight)
{
	// // 
	// vec3 v = normalize(inCommon.viewPos.xyz - pos);
	// vec3 l = -inLighting.lightDir.xyz;
	// vec3 h = normalize(l + v);
	// float dln = dot(l, normal);
	
	
	// // Diffuse, Specular Factors...
	// float df = max(dln, 0.0);
	// ds = pow(ds, 32);
	
	// // Smooth Specular and prevernt it for lighting back faces.
	// if (dln < 0.1)
	// {
		// ds = ds * max(mix(dln * 10.0, 1.0, 0.0), 0.0);
	// }
	
	// outLight.diffuse = df * inLighting.lightPower * inLighting.lightColor.rgb;
	// outLight.specular = ds * inLighting.lightPower * inLighting.lightColor.rgb;
}



//
void ComputeSunLight()
{
	
}



//
void ComputeDirectLights()
{
	// Compute Directional Lights...
	for (int i = 0; i < pointLightStartIndex; ++i)
	{
		
	}
	
	// Compute Point Lights...
	for (int i = pointLightStartIndex; i < spotLightStartIndex; ++i)
	{
		
	}
	
	// Compute Spot Lights...
	for (int i = spotLightStartIndex; i < numLights; ++i)
	{
		
	}
}






