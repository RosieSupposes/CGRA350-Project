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


void main() {

	vec3 defColour = uBrightness*uColor;
	// calculate lighting (hack)
	vec3 eye = normalize(-f_in.position);
	float light = abs(dot(normalize(f_in.normal), eye));

	vec3 lightPosition = vec3(0,5,0);
	float lightPower = 40;
	vec3 L = normalize(lightPosition - f_in.worldSpacePos);
	float dist = length(lightPosition - f_in.worldSpacePos);
	float lightIntensity = lightPower/(dist*dist);

	vec3 normal = normalize(f_in.normal);
	float lamb = max(dot(L, normal), 0);
	vec3 colour  = vec3(0);

	vec3 amb = vec3(0.1);
	
	
	vec3 view = normalize(-f_in.position);
	
	vec3 H = normalize(L+view);

	float nDotV = max(0, dot(normal,view));
	float nDotH = max(0, dot(normal,H));
	float vDotH = max(0, dot(H,view));
	float lDotV = max(0, dot(L,view));

	vec3 spec = vec3(1)*pow(nDotH,16); 
	vec3 diff = defColour * lamb * texture(uTexture, f_in.textureCoord).xyz;
	vec3 col = amb + (diff + spec)*uLightColour*lightIntensity;

	float lum = (0.299 * col.r + 0.587 * col.g + 0.114 * col.b);
	vec4 texCol = texture(uEffectTexture, gl_FragCoord.xy);
	float offset;
	vec2 coords;
	vec3 colBW = vec3(0);
	if(lum > 0.75)
	{
		coords = vec2((f_in.textureCoord.x)/4,f_in.textureCoord.y);
		colBW += texture(uEffectTexture, coords).xyz;
	}
	if(lum > 0.5)
	{
		coords = vec2((1+f_in.textureCoord.x)/4,f_in.textureCoord.y);
		colBW += texture(uEffectTexture, coords).xyz;
	}
	if(lum > 0.25)
	{
		coords = vec2((2+f_in.textureCoord.x)/4,f_in.textureCoord.y);
		colBW += texture(uEffectTexture, coords).xyz;
	}
	else
	{
		coords = vec2((3+f_in.textureCoord.x)/4,f_in.textureCoord.y);
		colBW += texture(uEffectTexture, coords).xyz;
	}
	fb_color = vec4(colBW, 1);
}