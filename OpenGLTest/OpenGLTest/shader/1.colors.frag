#version 330 core
out vec4 FragColor;

in vec2 Texture;
in vec3 Normal;
in vec3 FragPos;

uniform float iTime;
uniform sampler2D ourTexture;
uniform vec2 iResolution;
uniform vec3 lightPos;
uniform vec3 lightPos_pro;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;


float saturate(float x)
{
    return max(0.0, min(1.0, x));
}
vec3 saturate(vec3 vx)
{
    return vec3(max(0.0, min(1.0, vx.x)), max(0.0, min(1.0, vx.y)), max(0.0, min(1.0, vx.z)));
}
float lerp(float a, float b, float t)
{
	return a * (1.0 - t) + b * t;
}
vec4 lerp(vec4 a, vec4 b, float t)
{
	return a * (1.0 - t) + b * t;
}

void main()
{
    // ambient
    float ambientStrength = 0.12;
    vec3 ambient = ambientStrength * lightColor;
  	
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);//入射方向
	vec3 lightDir_pro = normalize(lightPos_pro - FragPos);//入射方向

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 reflectDir_pro = reflect(-lightDir_pro, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	float spec_pro = pow(max(dot(viewDir, reflectDir_pro), 0.0), 128);

	vec3 specular = specularStrength * lightColor * spec;
	vec3 specular_pro = specularStrength * lightColor * spec_pro;

    //vec2 uv = gl_FragCoord.xy / iResolution;
    vec2 uv = Texture / iResolution;

	//localPos
	vec3 AdjustLocalPos = saturate(vec3(FragPos.x, FragPos.y, FragPos.z)).xyz + 0.4;

	//Rim
	float softRim = 1.0 - saturate(dot(normalize(viewDir), Normal));// calculate a soft fresnel based on the view direction and the normals of the object
	float hardRim = round(softRim); // round it up for a harder edge

	//Emission
	vec4 _Color = vec4(100.0 / 255.0, 212.0 / 255.0, 238.0 / 255.0, 1);// 正蔚蓝色
	float _RimBrightness = 2.0;
	float _InnerRimOffSet = 2.5;
	vec4 Emission = _Color * lerp(hardRim, softRim, saturate(AdjustLocalPos.x + AdjustLocalPos.y)) * _RimBrightness;	 // lerp the emission from the hard rim to the softer one, based on the position

	float innerRim = _InnerRimOffSet + saturate(dot(normalize(viewDir), Normal));
	//Albedo
	vec4 _BottomColor = vec4(58.6 / 255.0, 0.0 / 255.0, 242.0 / 255.0, 1);// 紫色
	vec4 _TColor = vec4(0.0 / 255.0, 85.0 / 255.0, 144.0 / 255.0, 1);// 深蓝色
	float _Offset = 3.2;
	// pow加上0.7次方，可以让里面的inner深颜色范围更少，更浅一点
	// _BottomColor基本上没有用到
	vec4 Albedo = _Color * pow(innerRim, 0.7) * lerp(_BottomColor, _TColor, saturate(FragPos.y + _Offset)) + vec4(specular, 1.0) + vec4(specular_pro, 1.0);

	FragColor = Emission + Albedo;
} 