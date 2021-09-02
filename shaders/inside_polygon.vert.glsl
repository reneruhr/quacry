#version 330 core

layout (location = 0) in vec4 in_point;

uniform mat2 polygon_transform;

uniform mat4 pv;
uniform mat4 transform;

uniform vec2[8] shape;
uniform int n;

uniform float point_size;
uniform float alpha;
uniform float zdecay;
uniform float wdecay;
uniform vec4 zColor;
uniform vec4 wColor;

out vec4 vert_color;

float Wedge(in vec2 v, in vec2 w)
{
	return v.x*w.y-w.x*v.y;
}

bool InsideShape(in vec2 p)
{
	for(int i=0; i<n; i++){
		if(Wedge( shape[(i+1)%n]-shape[i], p-shape[i] )  < 0 ) return false;
	}
	return true;
}

float recz(float s)
{
	return min(zdecay/(s+0.01f),1.0f);
}

float recw(float s)
{
	return min(wdecay/(s+0.01f),1.0f);
}

void main()
{
	vec4 point = transform * in_point;
	vert_color = vec4(0.0f, 0.0f, 1.0f, alpha);
	if(InsideShape(point.zw)){
		vert_color = vec4(0.3,0.3,0,0.3)+ recz(abs(point.z))*zColor+recw(abs(point.w))*wColor;
	}

	// Now discard 4-dim data, only keep projection to 3-dim
	gl_Position = pv * vec4(point.xyz,1);
	gl_PointSize = point_size;
}
