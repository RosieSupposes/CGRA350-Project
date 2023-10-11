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


// mesh data
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

void main() {
	// set the screenspace position (needed for converting to fragment data)
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);
}