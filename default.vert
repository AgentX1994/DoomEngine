#version 150 core

in vec3 vert_position;
in vec2 in_uvs;

uniform mat4 mWorld;
uniform mat4 mView;
uniform mat4 mProj;

out vec2 uvs;

void main()
{
    gl_Position = mProj * mView * mWorld * vec4(vert_position, 1);
    uvs = in_uvs;
}
