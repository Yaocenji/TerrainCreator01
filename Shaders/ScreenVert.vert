#version 450 core

in vec2 pos;

out vec2 texCoord;

void main()
{
    texCoord = pos / 2 + vec2(0.5, 0.5);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
}
