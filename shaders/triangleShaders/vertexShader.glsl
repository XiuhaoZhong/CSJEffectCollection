#version 430

uniform mat4 rotateVec;

void main(void) {
	if (gl_VertexID == 0) {
		gl_Position = rotateVec * vec4(0.25, -0.25, 0.0, 1.0);
	} else if (gl_VertexID == 1) {
		gl_Position = rotateVec * vec4(-0.25, 0.25, 0.0, 1.0);
	} else {
		gl_Position = rotateVec * vec4(0.25, 0.25, 0.0, 1.0);
	}
}