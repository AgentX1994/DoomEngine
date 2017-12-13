#version 150 core

in vec2 uvs;

out vec4 outColor;
uniform sampler1D distances;

// Currently, a distance of 1.0 corresponds to half the screen as wall
// and a distance of 0.0 as the whole screen

// Some random colors to use
vec3 ceilColor = vec3(0.75,0.25,0.75);
vec3 wallColor = vec3(0.5, 0.1, 0.5);
vec3 floorColor = vec3(0.9, 0.5, 0.9);

void main()
{
    float dist = texture(distances, uvs.x).r;
    float upperThreshold = 1.0 - dist/4;    // dist = 0 -> 1, dist = 1 -> 0.75;
    float lowerThreshold = dist/4;          // dist = 0 -> 0, dist = 1 -> 0.25;
    
    float height = uvs.y;
    vec3 color;
    if (height > upperThreshold)
    {
        color = ceilColor;
    } else if (height > lowerThreshold)
    {
        color = wallColor;
    } else
    {
        color = floorColor;
    }
    outColor = vec4(color, 1.0);
}
