#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;


uniform float brushSize;
uniform vec4 brushColor;
uniform vec2 uv;
uniform bool painting;


void main()
{
   // Pixeles de la imagen TODOS E	
   ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
   if (painting)
   {
      // Pixeles de [0, 1] (normalizados)
      vec2 pixelCoords = vec2(texelCoord) / imageSize(imgOutput);
      float maxLength = length(imageSize(imgOutput));
      
      
      ivec2 uvPixels = ivec2(int((uv.x * imageSize(imgOutput))),int((uv.y * imageSize(imgOutput))));
      
      float distance = (distance(uvPixels, texelCoord));
     
      float percentage = smoothstep(0.0, brushSize, distance);	
      vec4 texelColor = imageLoad(imgOutput, texelCoord);
      vec3 finalColor = mix(texelColor.rgb, brushColor.rgb, 1.0 - percentage);
      
      
	  imageStore(imgOutput, texelCoord, vec4(finalColor,1.0));
   }
  
   

}