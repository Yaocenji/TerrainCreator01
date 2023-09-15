#version 450 core 

in vec4 worldPos;
in vec2 texCoord;
in vec4 clipPos;
in vec3 normal;

out vec4 FragColor;

layout (binding = 0, r32f) uniform image2D HeightField;

layout (binding = 1, rgba32f) uniform image2D ColorMap_01;
uniform int ColorMap_01_value;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

uniform sampler2D TerrainData;

uniform bool useHeightFieldBuffer;

void main(){
    vec2 sampleTexCoord = vec2(texCoord.xy * TerrainGrid);

    float height = worldPos.y * TerrainSize / TerrainHeight;

    // 采样buffer的itex
    ivec2 itexCoord = ivec2(texCoord.xy * TerrainGrid);

    // 采样基础色
    vec3 albedo;
    if (ColorMap_01_value != 0)
        albedo = imageLoad(ColorMap_01, itexCoord).rgb;
    else
        albedo = vec3(1, 1, 1);

    // 简单光照模型
    vec3 lightCol = vec3(1, 1, 1);
    vec3 lightDir = normalize(vec3(2.3, 3.5, 2.1));

    vec3 ansCol = albedo * lightCol * dot(normal, lightDir);

    FragColor = vec4(ansCol.xyz, 1.0);

    // 测试代码
    // FragColor = vec4(HeightFieldData / TerrainHeight, 1.0, 1.0, 1.0);
}