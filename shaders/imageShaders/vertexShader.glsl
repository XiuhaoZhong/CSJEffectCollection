#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCood;

out vec3 ourColor;
out vec2 TexCood;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(position, 1.0f);
	ourColor = color;
	TexCood = vec2(texCood.x, 1.0f - texCood.y);
}