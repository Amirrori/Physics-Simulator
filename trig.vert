#version 410 core
in vec3 vp;

void main() {
	gl_Position = vec4(vp.x + 0.5, vp.y, vp.z, 1.0);
}
