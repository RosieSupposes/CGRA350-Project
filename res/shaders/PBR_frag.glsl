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


const float pi = 3.14159;

void main() {
	vec3 defColour = uColor * uBrightness;
	vec3 amb = vec3(0.1);
	vec3 normal = normalize(f_in.normal);
	vec3 L = normalize(uLightPosition - f_in.worldSpacePos);
	float dist = length(uLightPosition - f_in.worldSpacePos);
	float lightIntensity = uLightPower/(dist*dist);
	vec3 view = normalize(-f_in.position);
	float lamb = max(dot(L, normal), 0);
	vec3 H = normalize(L+view);

	float nDotV = max(0, dot(normal,view));
	float nDotH = max(0, dot(normal,H));
	float vDotH = max(0, dot(H,view));
	float lDotV = max(0, dot(L,view));

	//Cook Torrance
	float alp_sq = uMetallic;
	float df_div = (nDotH*nDotH*(alp_sq-1.0)+1.0);
	float df = (alp_sq)/(df_div*df_div);

	float geo1 = 2*nDotH*nDotV/vDotH;
	float geo2 = 2*nDotH*lamb/vDotH;
	float geo = min(geo1, geo2);

	float n2 = 2.15;
	float f_zero = 0;
	float fre = f_zero * (1-f_zero)*pow(1-vDotH, 5);
	float rS = fre*df*geo/(pi*lamb*nDotV);

	vec3 spec = vec3(1)*rS;

	//Oren_nayer

	float p =0.8;
	float alpha_sq = uRoughness*uRoughness;
	float A = 1 - 0.5*alpha_sq/(alpha_sq+0.33) + 0.17*p*alpha_sq/(alpha_sq+0.13);
	A = A/pi;
	float B = 0.45*alpha_sq/(alpha_sq+0.09);
	B = B/pi;

	float s = lDotV - lamb*nDotV;
	float t = 1;
	if(s > 0)
	{
		t = max(lamb, nDotV);
	}

	
	vec3 oren_nayar = defColour * lamb * (A+B * s/t); 
	vec3 diff = oren_nayar * texture(uTexture, f_in.textureCoord).xyz * lightIntensity;
	
	vec3 col = amb + (diff + spec)*uLightColour*lightIntensity;
  	fb_color = vec4(col, 1.0);
}