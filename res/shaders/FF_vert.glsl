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

// mesh data
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// model data (this must match the input of the vertex shader)
out VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} v_out;

void main() {

	// transform vertex data to viewspace
	v_out.position = (uViewMatrix * uModelMatrix * vec4(aPosition, 1)).xyz;
	v_out.normal = normalize((uViewMatrix * uModelMatrix * vec4(aNormal, 0)).xyz);
 	v_out.textureCoord = aTexCoord;
 
	// set the screenspace position (needed for converting to fragment data)
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);
}