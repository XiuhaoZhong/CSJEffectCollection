#version 430

in vec3 vNormal, vLightDir, vVertPos, vHalfVec;
in vec4 shadow_coord;

out vec4 fragColor;

struct PositionalLight {
	vec4 ambient, diffuse, specular;
	vec3 position;
};

struct Material {
	vec4 ambient, diffuse, specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform mat4 shadowMVP;
uniform int  inSelected;
layout (binding = 0) uniform sampler2DShadow shadowTex;

void main() {

	vec3 L = normalize(vLightDir);
	vec3 N = normalize(vNormal);
	vec3 V = normalize(-vVertPos);
	vec3 H = normalize(vHalfVec);

	float inShadow = textureProj(shadowTex, shadow_coord);
	
	vec4 color = globalAmbient * material.ambient + light.ambient * material.ambient;
	if (inShadow != 0.0) {
		color += light.diffuse * material.diffuse * max(dot(L, N), 0.0f) +
				 light.specular * material.specular * pow(max(dot(H, N), 0.0), material.shininess * 3.0);
	}	 

	fragColor = color;
}