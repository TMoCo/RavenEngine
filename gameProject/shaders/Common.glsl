// Stage Macros:
//		- Macro used to determine which stage we are in, used when you have a file that is shared between multiple stages.
//		- if the macro is not defined the define its default to zero.
#ifndef STAGE_VERTEX_SHADER
#define STAGE_VERTEX_SHADER 0
#endif

#ifndef STAGE_FRAGMENT_SHADER
#define STAGE_FRAGMENT_SHADER 0
#endif

#ifndef STAGE_GEOMETRY_SHADER
#define STAGE_GEOMETRY_SHADER 0
#endif

#ifndef STAGE_TESS_CONTROL_SHADER
#define STAGE_TESS_CONTROL_SHADER 0
#endif

#ifndef STAGE_TESS_EVALUATION_SHADER
#define STAGE_TESS_EVALUATION_SHADER 0
#endif



// Render Pass Macros:
//		- Macro used to determine which render pass we are currently in.
#ifndef RENDER_PASS_DEFERRED
#define RENDER_PASS_DEFERRED 0
#endif


#ifndef RENDER_PASS_FORWARD
#define RENDER_PASS_FORWARD 0
#endif




// Shader Type Macros:
//		- Macro used to determine what is the shader type.
#ifndef RENDER_SHADER_TYPE_OPAQUE
#define RENDER_SHADER_TYPE_OPAQUE 0
#endif


#ifndef RENDER_SHADER_TYPE_MASKED
#define RENDER_SHADER_TYPE_MASKED 0
#endif


#ifndef RENDER_SHADER_TYPE_TRANSLUCENT
#define RENDER_SHADER_TYPE_TRANSLUCENT 0
#endif


#ifndef RENDER_SHADER_TYPE_POSTPROCESSING
#define RENDER_SHADER_TYPE_POSTPROCESSING 0
#endif


#ifndef RENDER_SHADER_CUSTOM
#define RENDER_SHADER_CUSTOM 0
#endif







// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 








// General uniform data that can be used by all shaders
layout(std140) uniform CommonBlock
{
	// The View & Projection Matrix of the scene.
	mat4 viewProjMatrix;
	
	// The Inverse of View & Projection Matrix of the scene.
	mat4 viewProjMatrixInverse;
	
	// The View Direction of the scene.
	vec3 viewDir;
	
	// The View Position of the scene.
	vec3 viewPos;
	
	// The render viewport postion and size, X,Y: Position, and Z,W: Size.
	//     - Values Ranges [0, width], [0, Height]
	vec4 viewport;
	
	// The environment sun direction.
	vec4 sunDir;
	
	// The environment sun color(RGB) & power(A).
	vec4 sunColorAndPower;
	
	// X: Near Plane, Y: Far Plane.
	vec2 nearFar;
	
	// Application Time.
	float time;
	
} inCommon;


// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 


#define NEAR_VALUE inCommon.nearFar.x
#define FAR_VALUE inCommon.nearFar.y


// --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- --- -- 





// Compute world position from depth.
vec3 ComputeWorldPos(vec2 coord, float depth)
{
	depth = depth * 2.0 - 1.0;
	return (inCommon.viewProjMatrixInverse * vec4(coord, depth, 1.0)).xyz;
}


// Convert Depth map value to a linear depth in eye-space.
float DepthToLinaer(float depth) 
{
    depth = depth * 2.0 - 1.0;
    return (2.0 * NEAR_VALUE * FAR_VALUE) / (FAR_VALUE + NEAR_VALUE - depth * (FAR_VALUE - NEAR_VALUE));	
}









