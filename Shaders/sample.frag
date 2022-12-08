#version 330 core

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D norm_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

//uniform vec4 circleColor;

out vec4 FragColor;

void main()
{


	vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1)
	{
		discard;
	}
	vec3 texYae = texture(tex1, texCoord).rgb;	
	vec3 normal = texture(norm_tex, texCoord).rgb;


	vec3 color = mix(texYae, normal, 0.01);

	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0f);
	 
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr * color;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1f), specPhong);
	
	vec3 specCol = spec * specStr * color;

	FragColor = vec4(specCol + diffuse + ambientCol, 0.7f) * pixelColor;

	
}