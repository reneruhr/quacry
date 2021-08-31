#version 330 core

layout (location = 0) in  vec2 vertex;

uniform mat4 transform;
uniform float depth;
uniform vec4 color;
out vec4 vColor;

void main()
{
    gl_Position= transform * vec4(vertex,depth,1);
	vColor = color;	
}
