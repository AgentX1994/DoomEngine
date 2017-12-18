#version 150 core

in vec4 position;
out vec2 uvs;

void main()
{
    gl_Position = vec4(position.xy, 0, 1);
    uvs = position.zw;
}
