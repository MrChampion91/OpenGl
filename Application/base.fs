#version 330 core

out vec4 FragColor;
in vec3 ourColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec3 viewPos;//viev position for reflection

uniform vec3 objectColor;
uniform vec3 lightPos; //position of light sorse
uniform vec3 lightColor; // pover of light sourse

void main()
{
    // ambien light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // dif light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // reflection
    float specularStrength = 0.9;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);

};