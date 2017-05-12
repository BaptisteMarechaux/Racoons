#version 330 core

// Notice that the "1" here equals the "1" in glVertexAttribPointer
in vec4 fragmentColor;

out vec4 color;

void main () {
    color = fragmentColor;
}