#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;	

out vec3 TexCoords;

uniform mat4 u_ViewProjection;

void main()
{
    TexCoords = a_Position;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

out vec4 color;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, TexCoords);
}
