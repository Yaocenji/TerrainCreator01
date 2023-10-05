#version 450 core

#define PI 3.14159265358979323846

in vec3 normal;

out vec4 FragColor;

void main(){
    FragColor = vec4(normal.xyz, 1.0);
}
