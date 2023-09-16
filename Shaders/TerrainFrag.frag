#version 450 core 

in vec4 worldPos;
in vec2 texCoord;
in vec4 clipPos;
in vec3 normal;

out vec4 FragColor;

layout (binding = 0, r32f) uniform image2D HeightField;

layout (binding = 1, rgba32f) uniform image2D ColorMap0;
uniform int ColorMap0_value;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

uniform sampler2D TerrainData;

uniform bool useHeightFieldBuffer;

void main(){
    
    ivec2 sampleTexCoord = ivec2(texCoord.xy * TerrainGrid);

    float height = worldPos.y * TerrainSize / TerrainHeight;

    vec3 albedo;
    if (ColorMap0_value != 0.0){
        albedo = imageLoad(ColorMap0, sampleTexCoord).rgb;
    }else {
        albedo = vec3(1, 1, 1);
    }

    // 简单光照模型
    vec3 lightCol = vec3(1, 1, 1);
    vec3 lightDir = normalize(vec3(2.3, 3.5, 2.1));

    vec3 ansCol = albedo * lightCol * dot(normal, lightDir);

    FragColor = vec4(ansCol.xyz, 1.0);

    // 测试代码
    // float tans = ColorMap0_value / 15.0;
    // FragColor = vec4(albedo.xyz, 1.0);
}