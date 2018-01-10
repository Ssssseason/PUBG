#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D texture_diffuse1;
uniform sampler2D depthMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

float CalShadow(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float shadow;
    if(projCoords.z > 1.0){
			shadow = 0.0;
	}
	else {
		float closestDepth = texture(depthMap, projCoords.xy).r; 
		float currentDepth = projCoords.z;

		float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.005);
		shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	}

    return shadow;
}

void main() {    
    vec3 lightColor = vec3(1.0);
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal));

    float shininess = 100;
    float a = 0.3;
    float d = max(dot(lightDir, normal), 0);
    float s = 0.3 * pow(max(dot(reflectDir, viewDir), 0), shininess);
    
    vec3 ambient = a * lightColor;
    vec3 diffuse = d * lightColor;
    vec3 specular = s * lightColor;

    float shadow = CalShadow(FragPosLightSpace, lightDir, normal);       

    FragColor = vec4(((ambient + (1.0 - shadow) * (diffuse + specular)) * color), 1.0f);
    // FragColor = vec4(color, 1.0f);
}