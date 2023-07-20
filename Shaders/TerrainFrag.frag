#version 450 core 

in vec4 worldPos;

in vec2 texCoord;
in vec3 col;

out vec4 FragColor;

uniform int TerrainGrid;
uniform float TerrainSize;
uniform float TerrainHeight;

void main(){
    float height = worldPos.y * TerrainSize / TerrainHeight;
    vec3 ans = vec3(height);
    FragColor = vec4(ans.xyz, 1.0);
}