#version 330 core

layout(points) in;
layout(points, max_vertices = 1) out;

uniform mat4 transform;
uniform mat4 pv;

uniform float point_size;
uniform vec4 color;

in vec3 complement[];

uniform mat4 g11;
uniform vec4 g51;
uniform vec4 g15;
uniform float g55;

in vec3 internal[];
in vec2 nbs1[];
in vec3 nbs2[];

out vec4 vert_color;

vec2 Apply_g_AndProject(){
    return (g11*vec4(gl_in[0].gl_Position.xy, internal[0].xy)).xy + g51.xy * internal[0].z;
}

vec4 Project(in vec4 v)
{
    v.zw = vec2(0,1);
    return v;
}

void main()
{
    vert_color = vec4(0.3,0.8,0.2,1);
    //vec2 v = Apply_g_AndProject();
    vec2 v = gl_in[0].gl_Position.xy;
    vec4 point = transform * vec4(v,0,1);

    gl_Position =  pv * point;
    gl_PointSize = point_size;
    EmitVertex();
    EndPrimitive();
}
