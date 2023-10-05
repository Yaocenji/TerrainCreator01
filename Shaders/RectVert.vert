#version 450 core 

#define PI 3.14159265

layout(location = 0)in vec2 pos;

out vec4 worldPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;


void main(){
    // 根据传入顶点信息构建三维顶点
    vec4 modelPos;

    modelPos = vec4(pos.x, 0, pos.y, 1.0);
    normal = vec3(0, 1, 0);

    // 乘以mvp矩阵
    worldPos = model * modelPos;
    gl_Position = proj * view * worldPos;
}