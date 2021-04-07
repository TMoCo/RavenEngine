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







// General uniform data that can be used by all shaders
layout(std140) uniform CommonBlock
{
	// The View Direction.
	vec3 viewDir;
	
	// The View Position.
	vec3 viewPos;
	
	// Application Time.
	float time;
	
} inCommon;












