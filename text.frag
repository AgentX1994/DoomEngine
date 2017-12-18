#version 150 core

in vec2 uvs;
uniform sampler2D glyph;
uniform vec4 color;

out vec4 outColor;

void main()
{
    outColor = color * vec4(1,1,1, texture(glyph, uvs).r);
}

