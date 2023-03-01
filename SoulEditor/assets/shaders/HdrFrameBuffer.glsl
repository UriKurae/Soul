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
out vec4 color;
  
in vec2 v_TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{ 
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, v_TexCoords).rgb;
   
    if (hdr)
    {
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        result = pow(result, vec3(1.0 / gamma));
        color = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        color = vec4(result, 1.0);
    }
    
}