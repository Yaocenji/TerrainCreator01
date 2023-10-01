#version 450 core

uniform sampler2D colorBuffer;
uniform sampler2D depthBuffer;

layout (binding = 0, rgba32f) uniform image2D RTTarget;

in vec2 texCoord;

out vec4 FragColor;

uniform int screenWidth;
uniform int screenHeight;

void main()
{
    // 首先获得该像素的整数索引
    int xIndex = int(texCoord.x * screenWidth);
    int yIndex = int(texCoord.y * screenHeight);
    vec4 color = imageLoad(RTTarget, ivec2(xIndex, yIndex));
    color.x = color.x / (color.x + 1.0);
    color.y = color.y / (color.y + 1.0);
    color.z = color.z / (color.z + 1.0);
    // vec3 ans = texture(colorBuffer, texCoord).rgb;
    // FragColor = vec4(ans.xyz, 1);

    FragColor = color;

}
