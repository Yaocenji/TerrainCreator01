#version 450 core 

in vec4 worldPos;

in vec2 texCoord;

in vec4 clipPos;

out vec4 FragColor;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

void main(){
    float height = worldPos.y * TerrainSize / TerrainHeight;
    FragColor = vec4(0.5, height, height, 1.0);
    vec4 ansCol = vec4((clipPos.xy / clipPos.a) * 0.5 + 0.5, 0.0, 1.0);
    // FragColor = ansCol;
}