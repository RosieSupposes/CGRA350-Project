#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform float uBrightness;
uniform sampler2D texture;
uniform float uMetallic;
uniform float uRoughness;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
	// calculate lighting (hack)
	vec3 eye = normalize(-f_in.position);
	float light = abs(dot(normalize(f_in.normal), eye));
	vec3 color = mix(uColor / 4, uColor*uBrightness, light);
	float lum = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
	// output to the frambuffer
	fb_color = vec4(lum, lum, lum, 1);
}