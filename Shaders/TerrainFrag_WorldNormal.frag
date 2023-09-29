#version 450 core

#define PI 3.14159265358979323846

in vec4 worldPos;
in vec2 texCoord;
in vec4 clipPos;
in vec3 normal;
in float realHeight;

out vec4 FragColor;

void main(){
    FragColor = vec4(normal.xyz, 1.0);
}
