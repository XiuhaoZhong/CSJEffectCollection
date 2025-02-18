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

float lookup(float x, float y) {
	float t = textureProj(shadowTex, shadow_coord + vec4(x * 0.001 * shadow_coord.w,
														 y * 0.001 * shadow_coord.w,
														 -0.01, 0.0));
	
	return t;
}

float softShadowOn = 0.0;

void main() {
	vec3 L = normalize(vLightDir);
	vec3 N = normalize(vNormal);
	vec3 V = normalize(-vVertPos);
	vec3 H = normalize(vHalfVec);

	// hard shadow.
	/*
	float inShadow = textureProj(shadowTex, shadow_coord);
	vec4 color = globalAmbient * material.ambient + light.ambient * material.ambient;
	if (inShadow >= 0.1) {
		color += light.diffuse * material.diffuse * max(dot(L, N), 0.0f) +
					light.specular * material.specular * pow(max(dot(H, N), 0.0), material.shininess * 3.0);
	}	 

	fragColor = color;
	*/

	float shadowFactor = 0.0f;
	// get 4 texels around current texel, generate soft shadow.
	
	float swidth = 2.5;
	vec2 o = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
	shadowFactor += lookup(-1.5 * swidth + o.x,  1.5 * swidth - o.y);
	shadowFactor += lookup(-1.5 * swidth + o.x, -0.5 * swidth - o.y);
	shadowFactor += lookup( 0.5 * swidth + o.x,  1.5 * swidth - o.y);
	shadowFactor += lookup( 0.5 * swidth + o.x, -0.5 * swidth - o.y);
	shadowFactor = shadowFactor / 4.0;
	
	// get 64 texels around current texel. generate soft shadow.
	/*
	float width = 2.5;
	float endp = width * 3.0 + width / 2.0;
	for (float m = -endp ; m <= endp ; m = m + width) {
		for (float n = -endp ; n <= endp ; n = n + width) {
			shadowFactor += lookup(m, n);
		}	
	}
	shadowFactor = shadowFactor / 64.0;
	*/

	vec4 shadowColor = globalAmbient * material.ambient +
					   light.ambient * material.ambient;

	vec4 lightedColor = light.diffuse * material.diffuse * max(dot(L, N), 0.0) +
						light.specular * material.specular * pow(max(dot(H, N), 0.0), material.shininess * 3.0);
	
	fragColor = vec4((shadowColor.xyz + shadowFactor * (lightedColor.xyz)), 1.0f);
	
}