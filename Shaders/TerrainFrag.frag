#version 450 core 

in vec4 worldPos;

in vec2 texCoord;

out vec4 FragColor;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

void main(){
    float height = worldPos.y * TerrainSize / TerrainHeight;
    FragColor = vec4(0.5, height, height, 1.0);
}