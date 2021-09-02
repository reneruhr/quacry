#version 330 core

layout (location = 0) in  vec4 in_point;

uniform mat4 shape_transform;

uniform mat4 pv;
uniform mat4 transform;

uniform float point_size;
uniform float alpha;

uniform vec4 zColor;
uniform vec4 wColor;
uniform float depth;

uniform vec2[8] shape;
uniform int n;

out vec4 vert_color;


float wedge(in vec2 v, in vec2 w)
{
	return v.x*w.y-w.x*v.y;
}

bool InsideShape(in vec2 p)
{
	for(int i=0; i<n; i++){
		if(wedge( shape[(i+1)%n]-shape[i], p-shape[i] )  < 0 ) return false;
	}
	return true;
}

void main()
{
	vec4 point = transform * in_point;
	vert_color = vec4(1,1,1,0);

	if(InsideShape(point.zw)){
		vert_color = vec4(0,0,1,1);
		point.xy = point.zw;
		point.zw = vec2(depth,1.0f);
	}

	gl_Position = shape_transform*point;
	gl_PointSize = point_size;
}
