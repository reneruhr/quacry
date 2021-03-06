#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 20) out;

uniform mat4 transform;
uniform mat4 pv;
uniform vec4 color;

uniform mat4 g11;
uniform vec4 g5c;
uniform vec4 g5r;
uniform float g55;

in vec3 internal[];
in vec2 nbs1[];
in vec3 nbs2[];

out vec4 vert_color;

vec2 Apply_g_AndProject(){
    return (g11*vec4(gl_in[0].gl_Position.xy, internal[0].xy)).xy + g5c.xy * internal[0].z;
}

vec2 ConstructNeighbor(int coord){
 if(coord < 4) return g11[coord].xy;
 else return g5c.xy;
}

vec4 Project(in vec4 v)
{
    v.zw = vec2(0,1);
    return v;
}

void main()
{
    vec2 v = Apply_g_AndProject();

    vec4 point = transform * vec4(v,0,1);
    vec4 pv_point = pv*point;
    vert_color = vec4(1,0,0,1);

    for(int i = 0; i<5; i++){
        if(i<2){
            if( abs(abs(nbs1[0][i])-1.f) < 0.1f ) {
                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv_point + pv*transform*vec4( nbs1[0][i] * ConstructNeighbor(i), 0,0 );
                EmitVertex();
                EndPrimitive();
            }
            else if( abs(nbs1[0][i]-2.f) < 0.1f) {
                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv_point + pv*transform*vec4( -ConstructNeighbor(i), 0,0 );
                EmitVertex();
                EndPrimitive();

                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv_point + pv*transform*vec4(  ConstructNeighbor(i), 0,0 );
                EmitVertex();
                EndPrimitive();
            }
        }
        else{
            int j = i-2;
            if( abs(abs(nbs2[0][j])-1) < 0.001) {
                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv_point + pv*transform*vec4( nbs2[0][j] * ConstructNeighbor(i), 0,0 );
                EmitVertex();
                EndPrimitive();
            }
            else if( abs(abs(nbs2[0][j])-2) < 0.001) {
                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv_point + pv*transform*vec4( -ConstructNeighbor(i), 0,0 );
                EmitVertex();
                EndPrimitive();

                gl_Position = pv_point;
                EmitVertex();
                gl_Position = pv_point + pv*transform*vec4(  ConstructNeighbor(i), 0,0 );
                EmitVertex();
                EndPrimitive();
            }
        }
    }
}
