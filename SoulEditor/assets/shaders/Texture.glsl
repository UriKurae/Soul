#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;	
layout(location = 1) in vec3 a_Normal;	
layout(location = 2) in vec2 a_TexCoord;	

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FacePos;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_TexCoord = a_TexCoord;
    FacePos = vec3(u_Transform * vec4(a_Position, 1.0));
}

#type fragment
#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 4

out vec4 color;

in vec2 v_TexCoord;
in vec3 Normal;
in vec3 FacePos;

uniform vec3 camPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int totalPointLights;
uniform Material material;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - FacePos);

    vec3 totalLight = CalcDirLight(dirLight, norm, viewDir);
    
    for(int i = 0; i < totalPointLights; ++i)
        totalLight += CalcPointLight(pointLights[i], norm, FacePos, viewDir);
    
    color = vec4(totalLight, 1.0);
    float gamma = 2.2;
    color.rgb = pow(color.rgb, vec3(1.0/gamma));
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
   
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 facePos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - facePos);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));

    vec3 halfWayDir = normalize(lightDir + viewDir);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfWayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

    float distance = length(light.position - facePos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation = 1.0 / distance;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}