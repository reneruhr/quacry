#version 330 core

layout (location = 0) in vec2 in_physical;
layout (location = 1) in vec3 in_internal;

uniform bool Space;
const bool Physical = false;

out vec3 complement;

void main()
{
 if(Space == Physical){	
	complement = in_internal;
	gl_Position = vec4(in_physical, 0, 1);
}else{
	complement = vec3(in_physical, 0);
	gl_Position = vec4(in_internal, 1);
}
}
