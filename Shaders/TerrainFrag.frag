#version 450 core 

in vec4 worldPos;
in vec2 texCoord;
in vec4 clipPos;
in vec3 normal;

out vec4 FragColor;

layout (binding = 0, rgba32f) uniform image2D HeightField;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

uniform sampler2D TerrainData;

uniform bool useHeightFieldBuffer;                                                 


void main(){
    vec2 sampleTexCoord = vec2(texCoord.xy * TerrainGrid);

    float height = worldPos.y * TerrainSize / TerrainHeight;

    // 简单光照模型
    vec3 lightCol = vec3(1, 1, 1);
    vec3 lightDir = normalize(vec3(2.3, 3.5, 2.1));

    vec3 ansCol = lightCol * dot(normal, lightDir);

    FragColor = vec4(ansCol.xyz, 1.0);
}