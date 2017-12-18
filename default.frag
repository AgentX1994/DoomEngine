#version 150 core

in vec2 uvs;

out vec4 outColor;
uniform sampler1D distances;
uniform sampler1D colors;

// Currently, a distance of 1.0 corresponds to half the screen as wall
// and a distance of 0.0 as the whole screen

// Some random colors to use
vec4 ceilColor  = vec4(0.4, 0.5, 0.9, 1.0);
vec4 floorColor = vec4(0.5, 0.5, 0.5, 1.0);

const float dist_to_viewport = 0.1;
const float wall_height = 3;

void main()
{
    float dist = texture(distances, uvs.x).r;
    float wall_size = wall_height * dist_to_viewport / dist;
    float upperThreshold = 0.5 + wall_size;
    float lowerThreshold = 0.5 - wall_size;
 
    if (isinf(dist))
    {
        upperThreshold = 0.5;
        lowerThreshold = 0.5;
    }
    vec4 wallColor = texture(colors, uvs.x);

    float height = uvs.y;
    vec4 color;
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
    outColor = color;
}
