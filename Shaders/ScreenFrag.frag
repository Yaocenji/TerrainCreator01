#version 450 core

uniform sampler2D colorBuffer;
uniform sampler2D depthBuffer;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
    FragColor = vec4(texCoord.xy, 1.0, 1.0);
    vec3 ans = texture(colorBuffer, texCoord).rgb;
    FragColor = vec4(ans.xyz, 1);
}
