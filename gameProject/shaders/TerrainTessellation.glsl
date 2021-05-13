#version 450 core




#if STAGE_TESS_CONTROL_SHADER

// 
layout (vertices = 4) out;



// Vertex Shader Output.
in VertexOutput
{
	vec3 position;
} inTessCtrl[];



// Tess Control Output.
out TessCtrlOutput
{
	vec3 position;
} outTessCtrl[];




// ComputeTessLevel based on edge length in screen space
float ComputeTessLevel(vec4 v1, vec4 v2)
{
    vec4 p1 = v1;
    vec4 p2 = v2;
    p1 = p1 / p1.w;
    p2 = p2 / p2.w;
    float l = length( 0.5 * inCommon.viewport.zw * ( p1.xy - p2.xy ) );
	
#if RENDER_SHADER_TYPE_SHADOW
	return clamp( l / 16, 1.0, 32.0 );
#else
    return clamp( l / 16, 1.0, 64.0 );
#endif
}



void main()
{
	 // Set the control points of the output patch
    outTessCtrl[gl_InvocationID].position = inTessCtrl[gl_InvocationID].position;
	


	vec4 pos0 = inCommon.viewProjMatrix * vec4(inTessCtrl[0].position, 1.0);
	vec4 pos1 = inCommon.viewProjMatrix * vec4(inTessCtrl[1].position, 1.0);
	vec4 pos2 = inCommon.viewProjMatrix * vec4(inTessCtrl[2].position, 1.0);
	vec4 pos3 = inCommon.viewProjMatrix * vec4(inTessCtrl[3].position, 1.0);
	
	// Calculate the tessellation levels
    gl_TessLevelOuter[0] = ComputeTessLevel(pos0, pos2);
    gl_TessLevelOuter[1] = ComputeTessLevel(pos0, pos1);
    gl_TessLevelOuter[2] = ComputeTessLevel(pos1, pos3);
	gl_TessLevelOuter[3] = ComputeTessLevel(pos2, pos3);
    gl_TessLevelInner[0] = max(gl_TessLevelOuter[1], gl_TessLevelOuter[3]);
	gl_TessLevelInner[1] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[2]);

}




#endif




#if STAGE_TESS_EVALUATION_SHADER


layout(quads, fractional_even_spacing, cw) in;


// Tess Control Output.
in TessCtrlOutput
{
	vec3 position;
} inTessEval[];


// Tess Evaluation Output.
out TessEvalOutput
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
} outTessEval;



// Bin Data Uniform 
layout(std140) uniform TerrainBinBlock
{	
	// Scale of the entire terrain.
	vec2 inScale;
	
	// Min/Max height of the terrain.
	vec2 inHeight;

	// Terrain Bin Offset.
	vec2 inOffset;
	
	// Terrain UV Scaling.
	vec2 inUVScale;
	
};




// Height Map Texture.
uniform sampler2D inHeightMap;




vec2 Evaluate2D(vec2 v0, vec2 v1, vec2 v2, vec2 v3)
{
	vec2 vx0 = mix(v0, v1, gl_TessCoord.x);
	vec2 vx1 = mix(v2, v3, gl_TessCoord.x);
    return mix(vx0, vx1, gl_TessCoord.y);
}



vec3 Evaluate3D(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
{
	vec3 vx0 = mix(v0, v1, gl_TessCoord.x);
	vec3 vx1 = mix(v2, v3, gl_TessCoord.x);
    return mix(vx0, vx1, gl_TessCoord.y);
}



void ComputeNormal()
{
	float level = 3.5;
	
	vec2 texCoord = outTessEval.texCoord;
	vec2 texelSize = 1.0 / textureSize(inHeightMap, int(level));
	
	float h0 = textureLod(inHeightMap, texCoord - vec2(texelSize.x, 0.0), level).r * (inHeight.y - inHeight.x) + inHeight.x;
	float h1 = textureLod(inHeightMap, texCoord + vec2(texelSize.x, 0.0), level).r * (inHeight.y - inHeight.x) + inHeight.x;
	
	float h2 = textureLod(inHeightMap, texCoord - vec2(0.0, texelSize.y), level).r * (inHeight.y - inHeight.x) + inHeight.x; 
	float h3 = textureLod(inHeightMap, texCoord + vec2(0.0, texelSize.y), level).r * (inHeight.y - inHeight.x) + inHeight.x; 
	
	outTessEval.normal = normalize( vec3(h0 - h1, 3.7, h2 - h3) * 0.5 );
	outTessEval.tangent = normalize( vec3(h0 - h1, 0.0, 0.0) * 0.5 );
}



void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    outTessEval.position = Evaluate3D(inTessEval[0].position, inTessEval[1].position, inTessEval[2].position, inTessEval[3].position);
	outTessEval.texCoord = vec2(outTessEval.position.x / inScale.x, outTessEval.position.z / inScale.y);;
	outTessEval.position.y = texture(inHeightMap, outTessEval.texCoord).r * (inHeight.y - inHeight.x) + inHeight.x; 
	
	
#if RENDER_SHADER_TYPE_SHADOW
	gl_Position = inShadowViewProj * vec4(outTessEval.position, 1.0);
	gl_Position.z += 0.1;
#else
	ComputeNormal();
	
	gl_Position = inCommon.viewProjMatrix * vec4(outTessEval.position, 1.0);
#endif

}


#endif












