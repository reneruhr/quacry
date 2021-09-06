#version 330 core

layout (location = 0) in vec4 in_point;

void main()
{
	vec4 point = in_point;
	gl_Position = point;
}
