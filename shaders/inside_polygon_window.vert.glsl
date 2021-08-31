#version 330 core

layout (location = 0) in  vec4 inPoint;

uniform mat4 shape_transform;

uniform mat4 pv;
uniform mat4 transform;

uniform float point_size;
uniform float alpha;

uniform vec4 zColor;
uniform vec4 wColor;

uniform vec2[8] shape;

out vec4 vColor;

uniform float depth;


float wedge(vec2 v, vec2 w){
	return v.x*w.y-w.x*v.y;
}

bool InOctagon(vec2 p){
	for(int i=0; i<8; i++){
		if(wedge( shape[(i+1)%8]-shape[i], p-shape[i] )  < 0 ) return false;
	}
	return true;
}

void main()
{
    vec4 point = transform * inPoint;
	vColor = vec4(1,1,1,0);

   if(InOctagon(point.zw)){
		vColor = vec4(0,0,1,1);
		point.xy = point.zw;
		point.zw = vec2(depth,1.0f);
   }

	// Now discard 4-dim data, only keep projection to 3-dim
    gl_Position = shape_transform*point;
    gl_PointSize = point_size;
}