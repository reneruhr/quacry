#version 330 core

layout (location = 0) in  vec4 inPoint;

uniform mat2 polygon_transform;

uniform mat4 pv;
uniform mat4 transform;

uniform float point_size;
uniform float alpha;
uniform float zdecay;
uniform float wdecay;

uniform vec4 zColor;
uniform vec4 wColor;

uniform vec2[8] shape;

out vec4 vColor;


float wedge(vec2 v, vec2 w){
	return v.x*w.y-w.x*v.y;
}


bool InOctagon(vec2 p){
	for(int i=0; i<8; i++){
		if(wedge( shape[(i+1)%8]-shape[i], p-shape[i] )  < 0 ) return false;
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
    vec4 point = transform * inPoint;
	vColor = vec4(0.0f, 0.0f, 1.0f, alpha);

 	
   //float a = 1/length(octagon[0]);
   // float xval = abs(point.x);
   // vColor.x= a/min(abs(point.x), 0.2);
   if(InOctagon(point.zw)){
		// vColor.y= 1.0f-abs(point.w)/a;
		// vColor.z= 1.0f-abs(point.z)/a;
		vColor = vec4(0.3,0.3,0,0.3)+ recz(abs(point.z))*zColor+recw(abs(point.w))*wColor;
   }

	// Now discard 4-dim data, only keep projection to 3-dim
    gl_Position = pv * vec4(point.xyz,1);
    gl_PointSize = point_size;
}