//vertColorBlendClipped
#version 330

uniform mat4 projectionMatrix;

in vec4 vertPosition;
in vec4 vertColor;

out vec4 fragHPos;
out vec4 fragCol0;

////////////////////
// includes from cvCommonTransform.glsl
vec4 getThreeSpace(vec4); 
float smoothClip(float hardMin, float softMin, float softMax, float hardMax, float val);
///////////////////

void main() {
	vec4 threeSpace = getThreeSpace(vertPosition); 
	float savedW = threeSpace.w;
	threeSpace.w = 1.0;
	//threeSpace = cameraSpace;
	
	vec4 homogenousCoords = projectionMatrix * threeSpace; // homogenous clip space position
	//homogenousCoords.w = abs(homogenousCoords.w + savedW);
	//homogenousCoords.z = abs(homogenousCoords.z);
	fragHPos = homogenousCoords;
	//fragHPos = mul(megaMatrix, vertposition);
	//fragHPos = mul(megaMatrix, worldSpace);

	fragCol0.a = 0.2 * smoothClip(0.0, 0.1, 0.9, 1.0, savedW);
  
	//fragCol0.rgb = vertColor.xyz;
	//fragCol0.r = 1.0 - abs((savedW - 0.5) * 2.0);
	//fragCol0.b = mod(abs(threeSpace.x / 10), 1.0);
	fragCol0.r = mod(abs(vertPosition.x / 10.0), 2.0); //, 0.1);
	fragCol0.g = mod(abs(vertPosition.z / 10.0), 2.0); //, 0.1);
	fragCol0.b = mod(abs(vertPosition.w / 10.0), 2.0); //, 0.1);
	//fragCol0.g = vertColor.y;
	//fragCol0.r = abs(worldSpace.z / 10);
	//fragCol0.b = abs(worldSpace.w / 10);
	//fragCol0.xyz = vec3(savedW, savedW, savedW);
	//fragCol0.xyz = vertposition.xyz;
	//fragCol0.xyz = worldSpace.xyz;
	//fragCol0.xyz = homogenousCoords.xyz;
	//fragCol0.xyz = fragHPos.xyz;
	//fragCol0.rgb = max(fragCol0.rgb, vec3(0.1,0,0));
	fragCol0.r = max(fragCol0.r, 0.15);
	fragCol0.g = max(fragCol0.g, 0.05);
	fragCol0.b = max(fragCol0.b, 0.05);
	//fragCol0.rgb += vec3(0.1,0.1,0.1);
	//fragCol0.r = vertposition.w;

	//fragCol0.r = mod(abs(vertPosition.x / 13.0), 1.0);
	//fragCol0.g = mod(abs(vertPosition.z / 17.0), 1.0);
	//fragCol0.b = mod(abs(vertPosition.w / 11.0), 1.0);
	//fragCol0.r = max(fragCol0.r, 0.15);
	//fragCol0.g = max(fragCol0.g, 0.05);
	//fragCol0.b = max(fragCol0.b, 0.05);
	//float colorNorm = sqrt(12) / length(fragCol0);
	//vec3 ycol = vec3(0.2, 0.10, 0.15);
	//float ycolor = (mod(abs(vertPosition.y / 7.0), 2.0) - 1.0) * 0.5;
	//fragCol0.rgb += ycol * ycolor;
	//fragCol0.rgb = fragCol0.rgb * colorNorm;

	gl_Position = fragHPos;
}
