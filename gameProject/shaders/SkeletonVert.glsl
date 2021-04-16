#version 330 core



layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;

layout(location=3) in vec4 inWeight;
layout(location=4) in vec4 inIndices;


// Transform Unifrom Block.
layout(std140) uniform TransformBoneBlock
{
	mat4 inModelMatrix;
	mat4 inViewMatrix;
	mat4 inProjectionMatrix;
	mat4 bones[196];
};


// Vertex Shader Output.
out VertexOutput
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	
} vxOut;




// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 
// -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- -- --- -- 



mat4 getSkinMat()
{

	int index_0 = int(inIndices.x);
	int index_1 = int(inIndices.y);
	int index_2 = int(inIndices.z);
	int index_3 = int(inIndices.w);


	float weights_0 = inWeight.x;
	float weights_1 = inWeight.y;
	float weights_2 = inWeight.z;
	float weights_3 = inWeight.w;


	mat4 bone_0 = bones[index_0];
	mat4 bone_1 = bones[index_1];
	mat4 bone_2 = bones[index_2];
	mat4 bone_3 = bones[index_3];
	
	return bone_0 * weights_0 + bone_1 * weights_1 + bone_2 * weights_2 + bone_3 * weights_3;
}


void main()
{

	gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * (getSkinMat() * vec4(inPosition, 1.0));

	
	vxOut.position = (inModelMatrix * vec4(inPosition, 1.0)).xyz;
	vxOut.normal 	= (inModelMatrix * vec4(inNormal,   0.0)).xyz;
	vxOut.texCoord = inTexCoord;
}

