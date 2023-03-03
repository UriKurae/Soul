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

   // Pixeles de [0, 1] (normalizados)
   vec2 pixelCoords = vec2(texelCoord) / imageSize(imgOutput);

   
   
   // 256
   ivec2 uvPixels = ivec2(int((uv.x * imageSize(imgOutput))),int((uv.y * imageSize(imgOutput))));
   int minX = uvPixels.x - int(brushSize); // 226
   int maxX = uvPixels.x + int(brushSize); // 276

    int minY = uvPixels.y - int(brushSize); // 226
	int maxY = uvPixels.y + int(brushSize); // 276
   
	if (painting && texelCoord.x > minX && texelCoord.x < maxX && texelCoord.y > minY && texelCoord.y < maxY)
	{
		imageStore(imgOutput, texelCoord, brushColor);
	}
   

}