#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;

uniform vec4 brushColor;
uniform float brushSize;
uniform vec3 intersectionPoint;


void main()
{
    // Calculate the texture coordinate for the intersection point
    vec2 texCoord = vec2(intersectionPoint.x, intersectionPoint.y);

    // Calculate the distance from each texel to the intersection point
    ivec2 texel = ivec2(gl_GlobalInvocationID.xy);
    vec2 texelCenter = vec2(texel) + 0.5;
    float distance = length(texelCenter - texCoord) / brushSize;

    // Apply the brush stroke to the texture at the texel
    vec4 texelColor = imageLoad(imgOutput, texel);
    float alpha = smoothstep(0.0, 1.0, 1.0 - distance);
    texelColor.rgb = mix(texelColor.rgb, brushColor.rgb, alpha);
    imageStore(imgOutput, texel, texelColor);
}