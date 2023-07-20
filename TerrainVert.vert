#version 450 core 

in vec2 pos;

out vec4 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int TerrainGrid;
uniform float TerrainSize;

void main(){
    // 根据传入顶点信息构建三维顶点
    vec4 modelPos = vec4(pos.x, 0, pos.y, 1.0);
    // 乘以mvp矩阵
    worldPos = model * modelPos;
    vec4 clipPos = proj * view * worldPos;
    gl_Position = clipPos;
}