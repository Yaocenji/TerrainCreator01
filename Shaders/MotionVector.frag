#version 450 core

// 运动矢量shader

in vec4 worldPos;

out vec4 FragColor;

uniform mat4 last_view;
uniform mat4 last_proj;

uniform int screenWidth;
uniform int screenHeight;

void main(){
    vec2 this_fragIndex = vec2(gl_FragCoord.x, gl_FragCoord.y);
    // this_fragIndex = this_fragIndex * 0.5 + 0.5;

    vec4 last_FragCoord = last_proj * last_view * worldPos;
    last_FragCoord /= last_FragCoord.w;
    vec2 last_fragCoord = last_FragCoord.xy;
    last_fragCoord = last_fragCoord * 0.5 + 0.5;
    vec2 last_fragIndex = vec2(int(last_fragCoord.x * screenWidth) + 0.5, int(last_fragCoord.y * screenHeight) + 0.5);

    // FragColor = vec4(last_fragCoord - this_fragCoord, 1.0, 1.0);
    // TODO
    FragColor = vec4(last_fragIndex, 0, 1);
}
