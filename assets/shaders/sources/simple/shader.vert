#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoords;

out vec2 texCord;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(pos, 1.0);
    texCord = texCoords;
}