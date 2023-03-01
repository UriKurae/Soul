#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;

uniform vec2 brushPosition;
uniform float brushSize;
uniform vec4 brushColor;


void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    
    
    float distance = distance(vec2(pixelCoords), brushPosition);

  
    vec4 color = imageLoad(imgOutput, pixelCoords);

    
    imageStore(imgOutput, pixelCoords, color);

}