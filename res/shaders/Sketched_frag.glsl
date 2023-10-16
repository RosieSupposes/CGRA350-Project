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

 
 // viewspace data (this must match the output of the fragment shader)
 in VertexData {
 	vec3 worldSpacePos;
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;

/*

	vec3 a_coords_n = normalize(f_in.worldSpacePos);
	float lon = atan(a_coords_n.x, a_coords_n.z);
	float lat = acos(a_coords_n.y);
	vec2 tmp_coords = vec2(lon,lat) * (1.0 / 3.141592653);
	vec2 cords = vec2(tmp_coords.x * 0.5 + 0.5, 1 - tmp_coords.y);
*/


void main() {
	vec3 L = normalize(uLightPosition - f_in.worldSpacePos);
	float dist = length(uLightPosition - f_in.worldSpacePos);
	float lightIntensity = uLightPower/(dist*dist);
	vec3 defColour = uColor * uBrightness;
	float lamb = max(dot(L, normalize(f_in.normal)), 0);
	vec3 amb = vec3(0.1);

	// calculate lighting (hack)
 	vec3 eye = normalize(-f_in.position);
	vec3 color = amb + defColour*texture(uTexture, f_in.textureCoord).rgb*lamb*uLightColour*lightIntensity;
 	float lum = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
	vec4 col;
	//vec3 tmp = (1-texture(uEffectTexture, f_in.textureCoord)-lum).xyz*uC;
	//fb_color = 1-vec4(tmp,0);
	float offset;
	if(lum > 0.75)
	{
		offset = 0;
	}
	else if(lum > 0.5)
	{
		offset = 1;
	}
	else if(lum > 0.25)
	{
		offset = 2;
	}
	else
	{
		offset = 3;
	}
	vec2 coords = vec2((offset+f_in.textureCoord.x)/4,f_in.textureCoord.y);
	fb_color = texture(uEffectTexture, coords);
}