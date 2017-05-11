#version 330 core

attribute vec3 vertexPosition_modelspace;

uniform mat4 MVP;
uniform vec4 vertexColor;

out vec4 fragmentColor;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
    fragmentColor = vertexColor;
}