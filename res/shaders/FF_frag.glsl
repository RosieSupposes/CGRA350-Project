#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
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
	//vec3 eye = normalize(-f_in.position);
	//float light = abs(dot(normalize(f_in.normal), eye));
	//vec3 color = mix(uColor / 4, uColor*uBrightness, light);
	// output to the frambuffer
	fb_color = vec4(uColor*uBrightness, 1);
}