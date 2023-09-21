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

//uniform int testArray[125];

vec3 ColorWithHeight(float relativeHeight){
    vec3 col_0 = vec3(43, 115, 71) / 255.0;
    vec3 col_1 = vec3(255, 255, 255) / 255.0;
    vec3 ans = col_0 * (1 - relativeHeight) + col_1 * relativeHeight;
    return ans;
}
vec3 ColorWithSlope(vec3 norm){
    vec3 col_0 = vec3(43, 115, 71) / 255.0;
    vec3 col_1 = vec3(104, 101, 91) / 255.0;
    float k = abs(norm.y / length(norm.xz));
    vec3 ans = step(0.3, k) * col_0 + (1.0 - step(0.3, k)) * col_1;
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
        
        // albedo = ColorWithSlope(normal);
    }

    // 简单光照模型
    vec3 lightCol = vec3(1, 1, 1);
    vec3 lightDir = normalize(vec3(2.3, 3.5, 5.1));

    vec3 ansCol = albedo * lightCol * dot(normal, lightDir);

    FragColor = vec4(ansCol.xyz, 1.0);

    // 测试代码
   
//    FragColor = vec4(normal.xzy, 1.0);
}
