#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec3 vertexColor;
layout(location = 1) in vec3 vertexNormal_modelspace;

layout(location = 2) in vec4 vertexOcclusionColor;

uniform vec4 vertexColor;

out vec4 fragmentColor;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

uniform vec3 lightPos;
uniform float deltaTime;

uniform vec4 groundColor;
uniform vec4 skyColor;

void main()
{
	vec4 transparentColor = vec4(0, 0, 0, 0.5);
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	fragmentColor = vertexColor + vertexOcclusionColor;//+ transparentColor;
	Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(lightPos,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}