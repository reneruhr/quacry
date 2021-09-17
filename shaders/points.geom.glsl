#version 330 core

layout(points) in;
layout(points, max_vertices = 1) out;

uniform vec2[8] shape;
uniform vec2[8] pattern;
uniform int n;
uniform int n_pattern;

uniform mat4 transform;
uniform mat4 pv;
uniform mat4 basis;

uniform float point_size;
uniform float alpha;
uniform vec4 bound_zw;
uniform vec4 color_z;
uniform vec4 color_w;

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

const float eps = 0.0f;
bool InsidePattern(in vec2 p)
{
  if(n_pattern == 0) return true;
	for(int i=0; i<n_pattern; i++){
		if(Wedge( pattern[(i+1)%n_pattern]-pattern[i], p-pattern[i] )  < -eps ) return false;
	}
	return true;
}

vec4 Project(in vec4 v)
{
   v.zw = vec2(0,1);
   return v;
}


void normalize_zw(inout vec2 zw, in vec4 bound)
{
	zw = (zw + bound.xz ) / bound.yw;
}

void main()
{
  vec4 in_point = gl_in[0].gl_Position;
  vec4 point = transform * in_point;
  vert_color =  vec4(0.0f, 0.0f, 1.0f, alpha);
  if(InsideShape(point.zw)){
    if(n_pattern == 0){
      vert_color.w = 1;
      vert_color.xy = point.zw;
      normalize_zw(vert_color.xy, bound_zw);
    } else if(InsidePattern(point.zw)){
        vert_color = vec4(1); 
    } else
       vert_color = vec4(0.3,0.2,0.4,1);
  }

  gl_Position = pv * Project(point); 
  gl_PointSize = point_size;
  EmitVertex();
  EndPrimitive();
}
