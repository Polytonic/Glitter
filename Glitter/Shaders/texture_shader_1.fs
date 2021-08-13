#version 330 core

out vec4 FragColor;

in vec2 ourTexCoord; 
in vec3 ourColor; 

uniform sampler2D ourTexSampler;

void main()
{
   FragColor = texture(ourTexSampler,ourTexCoord);
};