#version 330 core

in vec4 vColor;

layout(location = 0) out vec3 color;

void main() 
{ 
   color = vColor.rgb;
} 

