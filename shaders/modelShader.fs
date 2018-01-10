#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{    
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

    FragColor = vec4(((ambient + diffuse + specular) * color), 1.0f);
    // FragColor = vec4(color, 1.0f);
}