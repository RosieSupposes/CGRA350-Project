#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uCameraPosition;
uniform vec3 uLightPosition;
uniform vec3 uLightColour;
uniform float uLightPower;
uniform vec3 uColor;
uniform float uBrightness;
uniform float uMetallic;
uniform float uRoughness;
uniform float uC;
uniform sampler2D uTexture;
uniform sampler2D uEffectTexture;
uniform sampler2D uDepthTexture;  


layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// model data (this must match the input of the vertex shader)
out VertexData {
	vec3 worldSpacePos;
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