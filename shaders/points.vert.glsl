#version 330 core

layout (location = 0) in vec4 in_point;

uniform mat4 pv;
uniform mat4 transform;
uniform WindowBlock {
	vec2 x;
	vec2 y;
	vec2 z;
	vec2 w;
};

out vec4 vert_color;

void main()
{
	vec4 point = transform * in_point;
	vert_color = vec4(1,1,0,1);

	if((z[0] <= point.z)  && (point.z <= z[1])  &&  (w[0] <= point.w) && (point.w <= w[1])){
		vert_color.x= abs(point.w)/10.0f;
		vert_color.z= 1.0f-abs(point.z)/5.0f;
		vert_color.y = abs(point.z)/5.0f;
	}

	gl_Position = pv * vec4(point.xyz,1);
}
