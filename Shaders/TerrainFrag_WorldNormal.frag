#version 450 core

#define PI 3.14159265358979323846

in vec4 worldPos;
in vec2 texCoord;
in vec4 clipPos;
in vec3 normal;
in float realHeight;
in float isEdge;

out vec4 FragColor;

uniform mat4 model;
uniform float TerrainSize;
uniform int TerrainGrid;

void main(){
    // // 先考察是否在边缘
    // // 先计算model后的地形边长
    // float modeled_TerrainSize = (model * vec4(TerrainSize, 0, 0, 1)).x;
    // // 计算model后的地块边长
    // float modeled_unit = modeled_TerrainSize / TerrainGrid;
    // // model后的高度
    // float modeled_height = (model * vec4(0, realHeight, 0, 1)).y;
    // // 计算这个片元是否在边缘上
    // bool flag_0;
    // if (worldPos.x <= -0.5 * (modeled_TerrainSize - 0.9 * modeled_unit) || worldPos.x >= 0.5 * (modeled_TerrainSize - 0.9 * modeled_unit) 
    //  || worldPos.z <= -0.5 * (modeled_TerrainSize - 0.9 * modeled_unit) || worldPos.z >= 0.5 * (modeled_TerrainSize - 0.9 * modeled_unit)){
    //     flag_0 = true;
    // } else flag_0 = false;
    // // 这个片元是否高度与高度场信息不符
    // bool flag_1;
    // if ((modeled_height - worldPos.y) >= 1e-8) flag_1 = true;
    // else flag_1 = false;

    vec3 ansNormal;
    // 判断该片元是否需要特别注意normal
    if (isEdge > 0){
        // x上界
        if (worldPos.z <= worldPos.x && worldPos.z > -worldPos.x){
            ansNormal = vec3(1, 0, 0);
        }
        // x下界
        else if (worldPos.z >= worldPos.x && worldPos.z < -worldPos.x){
            ansNormal = vec3(-1, 0, 0);
        }
        // z上界
        else if (worldPos.z >= -worldPos.x && worldPos.z > worldPos.x){
            ansNormal = vec3(0, 0, 1);
        }
        // z下界
        else if (worldPos.z <= -worldPos.x && worldPos.z < worldPos.x){
            ansNormal = vec3(0, 0, -1);
        }
    } else {
        ansNormal = normal;
    }
    
    FragColor = vec4(ansNormal.xyz, 1.0);
}
