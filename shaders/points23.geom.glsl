#version 330 core

layout(points) in;
layout(points, max_vertices = 1) out;

uniform mat4 transform;
uniform mat4 pv;

uniform float point_size;
uniform vec4 color;

in vec3 complement[];
out vec4 vert_color;

void main()
{
  vec4 in_point = gl_in[0].gl_Position;
  vec4 point = transform * in_point;
  vert_color = color;
  
  gl_Position = pv * point; 
  gl_PointSize = point_size;
  EmitVertex();
  EndPrimitive();
}
