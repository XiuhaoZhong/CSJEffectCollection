#version 430

in vec4 varyingColor;
out vec4 fragColor;

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;

struct PositionalLight {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material {
	vec4  ambient;
	vec4  diffuse;
	vec4  specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

// interpolate lighted color
// (interpolation of gl_Position is antomatic)

void main(void) {
	// normalize the light, normal, and view vectors.
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	// compute light reflection vector, with respect N.
	vec3 R = normalize(reflect(-L, N));

	// get the angle between the light and surface normal.
	float cosTheta = dot(L, N);

	// angle between the view vector and reflected light;
	float cosPhi = dot(V, R);

	// compute ADS contributes(per pixel).
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0f);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi, 0), material.shininess);

	fragColor = vec4((ambient + diffuse + specular), 1.0f);
}