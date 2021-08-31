#version 330 core

//in  vec2 texCoord;

in vec4 vColor;
out vec4 fColor;

//uniform sampler2D texture;

void main() 
{ 
   //fColor = textureLod( texture, texCoord, 0 );
   fColor = vColor;
} 

