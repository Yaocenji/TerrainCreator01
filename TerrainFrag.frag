#version 450 core 

in vec4 worldPos;

out vec4 FragColor;

uniform int TerrainGrid;
uniform float TerrainSize;

void main(){
    vec3 ans = vec3(worldPos.y, worldPos.y, worldPos.y);
    FragColor = vec4(ans.xyz, 1.0);
}