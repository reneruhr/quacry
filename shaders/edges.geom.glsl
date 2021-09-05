#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 16) out;

uniform vec2[8] shape;
uniform int n;

uniform mat4 transform;
uniform mat4 pv;
uniform mat4 basis;

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

out vec4 vert_color;

void main()
{
  vec4 point = gl_in[0].gl_Position;
  vec4 pv_point = pv*transform*point; pv_point.zw = vec2(0,1);
	if(InsideShape((transform * point).zw)){
    for(int i = 0; i<4; ++i){
        vec4 left = transform*(point+basis[i]);
        vec4 right = transform*(point-basis[i]);
        vert_color = vec4(1);
        if(InsideShape(left.zw)){
                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv * left;
                gl_Position.zw = vec2(0,1);
                EmitVertex();
                EndPrimitive();
                }
        if(InsideShape(right.zw)){
                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv * right;
                gl_Position.zw = vec2(0,1);
                EmitVertex();
                EndPrimitive();
                }
        }
    }
}