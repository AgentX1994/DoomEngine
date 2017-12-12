#version 150 core

in vec2 uvs;

out vec4 outColor;

/*
const float epsilon = 0.01;
const float UPPER_THRESHOLD = 1.0 - epsilon;
const float LOWER_THRESHOLD = epsilon;
*/

uniform sampler1D distances;

void main()
{
    float dist = texture(distances, uvs.x).r;
    outColor = vec4(dist, dist, dist, 1.0);
    /*
    if (uvs.x <= LOWER_THRESHOLD || 
        uvs.y <= LOWER_THRESHOLD || 
        uvs.x >= UPPER_THRESHOLD ||
        uvs.y >= UPPER_THRESHOLD) {
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else
    {
        outColor = vec4(uvs, 1.0, 1.0);
    }
    */
}
