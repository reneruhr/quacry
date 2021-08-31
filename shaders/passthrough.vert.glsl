#version 330 core

layout(location = 0) in vec3 vPosition;

out vec2 texCoords;

void main()
{
	gl_Position =  vec4(vPosition, 1.0f);
	texCoords = (vPosition.xy+vec2(1.0f)) / 2.0f;
}