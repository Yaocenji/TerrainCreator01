#version 450 core

uniform sampler2D colorBuffer;
uniform sampler2D depthBuffer;

layout (binding = 0, rgba32f) uniform image2D RTTarget;

in vec2 texCoord;

out vec4 FragColor;

uniform int screenWidth;
uniform int screenHeight;

uniform sampler2D RTDepth;

void main()
{
    // 首先获得该像素的整数索引
    int xIndex = int(texCoord.x * screenWidth);
    int yIndex = int(texCoord.y * screenHeight);
    vec4 color = imageLoad(RTTarget, ivec2(xIndex, yIndex));

    //  hdr到ldr
    color.x = color.x / (color.x + 1.0);
    color.y = color.y / (color.y + 1.0);
    color.z = color.z / (color.z + 1.0);
    color.w = color.w / (color.w + 1.0);
    
    // gamma校正
    color.x = pow(color.x, 2.2);
    color.y = pow(color.y, 2.2);
    color.z = pow(color.z, 2.2);
    color.a = pow(color.a, 2.2);

    FragColor = color;

    // TEST 
    // FragColor = vec4(color.aaa, 1);
}
