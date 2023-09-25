#version 450 core

#define PI 3.14159265358979323846

in vec4 worldPos;
in vec2 texCoord;
in vec4 clipPos;
in vec3 normal;
in float realHeight;

out vec4 FragColor;

layout (binding = 0, r32f) uniform image2D HeightField;

layout (binding = 1, rgba32f) uniform image2D ColorMap0;
uniform int ColorMap0_value;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

uniform sampler2D TerrainData;

uniform bool useHeightFieldBuffer;

uniform vec3 mainLightColor;
uniform float mainLightStrength;
uniform vec3 mainLightDir;

vec3 ColorWithHeight(float relativeHeight){
    vec3 col_0 = vec3(43, 115, 71) / 255.0;
    vec3 col_1 = vec3(255, 255, 255) / 255.0;
    vec3 ans = col_0 * (1 - relativeHeight) + col_1 * relativeHeight;
    return ans;
}
vec3 ColorWithSlope(vec3 norm){
    vec3 col_0 = vec3(11, 100, 43) / 255.0;
    vec3 col_1 = vec3(104, 101, 91) / 255.0;
    float k;
    k = abs(length(norm.xz) / norm.y);
    vec3 ans;
    if (k > 0.3){
        ans = col_1;
    } else {
        ans = col_0;
    }
    return ans;
}

void main(){
    ivec2 sampleTexCoord = ivec2(texCoord.xy * TerrainGrid);

    float height = realHeight / TerrainHeight;

    vec3 albedo;
    if (ColorMap0_value != 0.0){
        albedo = imageLoad(ColorMap0, sampleTexCoord).rgb;
    }else {
        albedo = ColorWithHeight(height);
        
        albedo = ColorWithSlope(normal);
    }

    // 简单光照模型
    vec3 mainLight = mainLightColor * mainLightStrength;

    vec3 ansCol = albedo * mainLight * max(dot(normal, mainLightDir), 0);

    ansCol = ansCol / (ansCol + vec3(1, 1, 1));
    ansCol = pow(ansCol, vec3(2.2));

    FragColor = vec4(ansCol.xyz, 1.0);

    // 测试代码
   
//    FragColor = vec4(1.0, texCoord.xy, 1.0);
}
