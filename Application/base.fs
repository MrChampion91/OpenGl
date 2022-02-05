#version 330 core

out vec4 FragColor;
in vec3 ourColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos; //position of light sorse
uniform sampler2D texture1;

uniform vec3 objectColor;
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


    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

};