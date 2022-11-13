#version 330 core

in vec2 texCord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCord);
}