#version 430
in vec3 ourColor;
in vec2 TexCood;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main() {
	color = mix(texture(ourTexture1, TexCood), texture(ourTexture2, TexCood), 0.2);
}