#version 330 core

layout (location = 0) in vec2 in_physical;
layout (location = 1) in vec3 in_internal;


layout (location = 2) in vec4 in_neighbors1;
layout (location = 3) in vec4 in_neighbors2;
layout (location = 4) in vec4 in_neighbors3;
layout (location = 5) in int  in_neighbors_count;

uniform bool Space;
const bool Physical = false;

out vec3 complement;
out vec4 nbs1, nbs2, nbs3;
out int nbs_count;

void main()
{
	if(Space == Physical){	
		complement = in_internal;
		gl_Position = vec4(in_physical, 0, 1);
	}else{
		complement = vec3(in_physical, 0);
		gl_Position = vec4(in_internal, 1);
	}

	nbs1 = in_neighbors1;
	nbs2 = in_neighbors2;
	nbs3 = in_neighbors3;
	nbs_count = in_neighbors_count;
}
