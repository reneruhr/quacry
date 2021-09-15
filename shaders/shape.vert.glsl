#version 330 core

layout (location = 0) in  vec2 vertex;

uniform mat4 transform;
uniform mat4 pv;
uniform float depth;
uniform vec4 color;
out vec4 vert_color;

void main()
{
    gl_Position= pv * transform * vec4(vertex,depth,1);
    vert_color = color;	
}
