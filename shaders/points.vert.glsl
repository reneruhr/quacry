#version 330 core

layout (location = 0) in  vec4 inPoint;

uniform mat4 pv;
uniform mat4 transform;
uniform WindowBlock {
	vec2 x;
	vec2 y;
	vec2 z;
	vec2 w;
};

out vec4 vColor;

void main()
{
    
    vec4 point = transform * inPoint;
	vColor = vec4(1,1,0,1);
   
   if((z[0] <= point.z)  && (point.z <= z[1])  &&  
   	  (w[0] <= point.w) && (point.w <= w[1])){
		vColor.x= abs(point.w)/10.0f;
		vColor.z= 1.0f-abs(point.z)/5.0f;
		vColor.y = abs(point.z)/5.0f;
	}

	// Now discard 4-dim data, only keep projection to 3-dim
    gl_Position = pv * vec4(point.xyz,1);

}
