#version 450 core 

#define PI 3.14159265

in vec2 pos;

layout (binding = 0, rgba32f) uniform image2D data;

out vec4 worldPos;
out vec2 texCoord;

out vec3 col;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

uniform sampler2D TerrainData;

uniform bool useHeightFieldBuffer;

void main(){
    vec2 rtexCoord = 2.0 * pos.xy / TerrainSize;
    texCoord = pos.xy / TerrainSize + vec2(0.5, 0.5);
    ivec2 itexCoord = ivec2(texCoord.xy * TerrainGrid);
    // 根据传入顶点信息构建三维顶点
    vec4 modelPos;
    if (useHeightFieldBuffer){
        float HeightFieldData = imageLoad(data, itexCoord).r;
        //HeightFieldData = texture(TerrainData, texCoord).x;
        modelPos = vec4(pos.x, HeightFieldData, pos.y, 1.0);

        if (HeightFieldData != 0) col = vec3(1, 0, 0);
    }
    else{
        float dist = length(rtexCoord);
        dist *= 3;
        float HeightData = 2 * TerrainHeight * 1.0 / sqrt(2.0 * PI) * exp(- dist * dist / 2.0);
        modelPos = vec4(pos.x, HeightData, pos.y, 1.0);
        col = vec3(HeightData / TerrainHeight, 0, 0);
    }
    // 乘以mvp矩阵
    worldPos = model * modelPos;
    vec4 clipPos = proj * view * worldPos;
    gl_Position = clipPos;
}