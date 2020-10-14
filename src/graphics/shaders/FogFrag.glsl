#version 450
layout (location = 2) out vec4 fragColor;

in vec3 vertNormal;
in vec2 vertUV;
in vec3 Position;


struct FogData
{
     vec3 color;
	 float maxDist;
	 float minDist;
};

uniform vec4 meshColor;
uniform sampler2D diffuseTex1;

uniform FogData Fog;

vec4 Phong()
{
	
	const vec4 ks = vec4(0.0, 0.0, 0.0,1.0);

    const vec4 kd = vec4(0.0, 0.0, 0.0,1.0); /// const means it cannot be changed just like C++

	const vec4 ka =  vec4(0.0,0.0,0.0,1.0);
	
	float diff = max(dot(vertNormal, vec3(1.0,1.0,1.0)), 0.0);
	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source

	vec3 reflection = normalize(reflect(-vec3(1.0,1.0,1.0), vertNormal));


	float spec = max(dot(Position, reflection), 0.0);

	if(diff > 0.0){
		spec = pow(spec,14.0);
	}
	
	return  ka + (diff * kd) + (spec * ks);	
}


void main()
{
    vec4 newcolor = vec4(Fog.color,0.6);
   float dist = length(Position.xyz);
   float fogFactor = (Fog.maxDist - dist) / (Fog.maxDist - Fog.minDist);


   vec4 color = mix(newcolor ,vec4(0.0 ,0.4 ,0.0 ,0.0),fogFactor);

	fragColor =  color;
}