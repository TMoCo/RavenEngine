#version 330 core






// Vertex Shader Output.
in VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} fgIn;



// Lighting Unifrom Block.
layout(std140) uniform LightingBlock
{
	// View
	vec4 viewDir;
	vec4 viewPos;
	
	// Dir-Light
	vec4 lightDir;
	vec4 lightColor;
	float lightPower;
	
} inLighting;



// Materail Unifrom Block.
layout(std140) uniform MaterailBlock
{
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
	float ambient;
	float alpha;
} inMaterail;


// Materail Texture Samplers...
uniform sampler2D inDiffuseTexture;
uniform sampler2D inSpecularTexture;



// Output...
out vec4 fragColor;




// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 






// Compute Directinal Light.
vec3 ComputeDirLight(vec3 pos, vec3 normal, vec4 difftex, vec4 spectex)
{
	// 
	vec3 v = normalize(inLighting.viewPos.xyz - pos);
	vec3 l = -inLighting.lightDir.xyz;
	vec3 h = normalize(l + v);
	float dln = dot(l, normal);
	
	
	// Diffuse, Specular Factors...
	float df = max(dln, 0.0) * inLighting.lightPower;
	float ds = max(dot(h, normal), 0.0);
	ds = pow(ds, inMaterail.shininess) * inLighting.lightPower;
	

	// Smooth Specular and prevernt it for lighting back faces.
	if (dln < 0.1)
	{
		ds = ds * max(mix(dln * 10.0, 1.0, 0.0), 0.0);
	}
	
	
	vec4 diffuse  = inMaterail.diffuse * difftex;
	vec4 specular = inMaterail.specular * spectex;
	
	vec4 lightFinal;
	lightFinal  = diffuse * inMaterail.ambient * inLighting.lightColor; // Ambient
	lightFinal += diffuse * df * inLighting.lightColor; // Diffuse
	lightFinal += specular * ds * inLighting.lightColor; // Specular
	lightFinal += inMaterail.emission; // Emission

	return lightFinal.rgb;
}




void main()
{
	vec3 normal = normalize(fgIn.normal);
	vec3 pos = fgIn.position;
	//vec4 difftex = texture(inDiffuseTexture, fgIn.texCoord);
	//vec4 spectex = texture(inSpecularTexture, fgIn.texCoord);
	vec4 difftex = vec4(1.0);
	vec4 spectex = vec4(1.0);
	
	fragColor.rgb = ComputeDirLight(pos, normal, difftex, spectex);
	fragColor.a = inMaterail.alpha;
}






