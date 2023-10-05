#version 450 core

#define PI 3.14159265358979323846

in vec4 worldPos;

out vec4 FragColor;

void main(){
    FragColor = vec4(worldPos.xyz, 1.0);
}
