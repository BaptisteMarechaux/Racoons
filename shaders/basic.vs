#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

out vec4 fragmentColor;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
    fragmentColor = vec4(1.0, 0.0, 0.0, 0.0);
}