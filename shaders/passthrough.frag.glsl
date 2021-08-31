#version 330 core


in vec2 texCoords;
out vec4 fColor;

uniform sampler2D tex;

void main() 
{ 
   //fColor = textureLod( texture, texCoord, 0 );
   //fColor = vColor;

   fColor = texture(tex, texCoords); 
   fColor.w = 1.0f;
} 

