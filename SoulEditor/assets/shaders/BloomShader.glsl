#type vertex
#version 430 core
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
    v_TexCoords = a_TexCoord;
}  

#type fragment
#version 430 core

out vec4 FragColor;
  
in vec2 v_TexCoords;

uniform sampler2D image;
uniform int iterations;
  
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);


void main()
{ 
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture(image, v_TexCoords).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < iterations/2; ++i)
        {
            result += texture(image, v_TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, v_TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < iterations/2; ++i)
        {
            result += texture(image, v_TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, v_TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
    
}