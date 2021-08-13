#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; 
out vec2 ourTexCoord;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   ourColor = aColor;
   ourTexCoord = aTexCoord;
};