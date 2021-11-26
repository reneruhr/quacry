#version 330 core
layout (location = 0) in vec2 in_physical;
layout (location = 1) in vec3 in_internal;

layout (location = 2) in vec2 in_neighbors;
layout (location = 3) in vec3 in_neighbors2;

uniform int Space;
uniform int Geometry;
const int Physical = 0;
const int Internal = 1;
const int Points = 0;
const int Edges  = 1;
const int PointsMat5 = 2;


out vec3 complement;
out vec3 internal;
out vec2 nbs1;
out vec3 nbs2;

void main()
{
	if(Geometry == Points){
		if (Space == Physical){
			complement = in_internal;
			gl_Position = vec4(in_physical, 0, 1);
		} else if (Space == Internal){
			complement = vec3(in_physical, 0);
			gl_Position = vec4(in_internal, 1);
		}
	}else if(Geometry == PointsMat5){
		if (Space == Physical){
			gl_Position = vec4(in_physical,0,0);
			internal = in_internal;
			nbs1 = in_neighbors;
			nbs2 = in_neighbors2;
		} else if (Space == Internal){
			complement = vec3(in_physical, 0);
			gl_Position = vec4(in_internal, 1);
		}
	}else if(Geometry == Edges) {
		gl_Position = vec4(in_physical,0,0);
		internal = in_internal;
		nbs1 = in_neighbors;
		nbs2 = in_neighbors2;
	}
}
