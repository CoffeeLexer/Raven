#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texPos;

out vec2 texCord;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    gl_PointSize = 2.5f;
    texCord = texPos;
}