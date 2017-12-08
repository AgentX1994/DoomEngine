#version 150 core

in vec3 vert_position;

uniform mat4 mWorld;
uniform mat4 mView;
uniform mat4 mProj;

void main()
{
    gl_Position = mProj * mView * mWorld * vec4(vert_position, 1);
}
