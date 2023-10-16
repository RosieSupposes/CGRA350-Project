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
 
 // viewspace data (this must match the output of the fragment shader)
 in VertexData {
 	vec3 worldSpacePos;
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

in vec4 gl_FragCoord;


// framebuffer output
out vec4 fb_color;

void main() {
	//Calculate edge
	vec3 defColour = uBrightness*uColor;
	// calculate lighting (hack)
	vec3 eye = normalize(-f_in.position);
	float light = abs(dot(normalize(f_in.normal), eye));

	vec3 aG = vec3(0.1, 0.1, 0.1);
	vec3 aL = vec3(1,1,1);
	vec3 dL = vec3(1,1,1);
	vec3 aM = uColor;
	vec3 dM = uColor;

	vec3 lightPosition = vec3(0,5,0);
	float lightPower = 40;
	vec3 L = normalize(lightPosition - f_in.worldSpacePos);
	float dist = length(lightPosition - f_in.worldSpacePos);
	float lightIntensity = lightPower/(dist*dist);

	vec3 normal = normalize(f_in.normal);
	vec3 lightColour = (aG + aL + dL)*dM;
	vec3 shadowColour = (aG + aL) * aM;
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
	vec3 diff = defColour * lamb*texture(uTexture, f_in.textureCoord).xyz;
	vec3 col = amb + (diff + spec)*uLightColour*lightIntensity;

	float lum = (0.299 * col.r + 0.587 * col.g + 0.114 * col.b);
	vec4 texCol = texture(uEffectTexture, gl_FragCoord.xy);
	if(lum >= texCol.r)
	{
		colour = lightColour;
	}
	else
	{
		colour = shadowColour;
	}
	fb_color = vec4(colour,1);
	//fb_color = texCol;
}