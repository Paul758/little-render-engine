#version 330 core

in vec2 uv;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

uniform vec2 texelSize;

const vec3 darkGrass = vec3(0.18, 0.32, 0.12);
const vec3 mediumGrass = vec3(0.28, 0.45, 0.18);
const vec3 lightGrass = vec3(0.42, 0.58, 0.24);

bool sameColor(vec4 a, vec4 b)
{
    const float tolerance = 0.01f;

    return distance(a.rgb, b.rgb) < tolerance; 
}

int grassType(vec3 color)
{
    float darkDistance = distance(color, darkGrass);

    float mediumDistance = distance(color, mediumGrass);

    float lightDistance = distance(color, lightGrass);

    if (darkDistance < mediumDistance && darkDistance < lightDistance)
    {
        return 0;
    }

    if (mediumDistance < lightDistance)
    {
        return 1;
    }

    return 2;
}

void main()
{
    vec4 center = texture(screenTexture, uv);

    vec4 neighbors[8] = vec4[](
        texture(screenTexture, uv + vec2(-texelSize.x, -texelSize.y)),
        texture(screenTexture, uv + vec2(0.0, -texelSize.y)),
        texture(screenTexture, uv + vec2(texelSize.x, -texelSize.y)),

        texture(screenTexture, uv + vec2(-texelSize.x, 0.0)),
        texture(screenTexture, uv + vec2(texelSize.x, 0.0)),

        texture(screenTexture, uv + vec2(-texelSize.x, texelSize.y)),
        texture(screenTexture, uv + vec2(0.0, texelSize.y)),
        texture(screenTexture, uv + vec2(texelSize.x, texelSize.y))
    );

    int matchingNeighbors = 0;

    int darkCount = 0;
    int mediumCount = 0;
    int lightCount = 0;

    for (int i = 0; i < 8; i++)
    {
        if (sameColor(center, neighbors[i]))
        {
            matchingNeighbors++;
        }

        int type = grassType(neighbors[i].rgb);

        if (type == 0)
        {
            darkCount++;
        }
        else if(type == 1)
        {
            mediumCount++;
        }
        else
        {
            lightCount++;
        }
    }
    
     int grassNeighborCount = darkCount + mediumCount + lightCount;

    if (grassNeighborCount == 0)
    {
        fragmentColor = center;
        return;
    }

    vec3 replacementColor;

    if (darkCount > mediumCount && darkCount > lightCount)
    {
        replacementColor = darkGrass;
    }
    else if (mediumCount > lightCount)
    {
        replacementColor = mediumGrass;
    }
    else
    {
        replacementColor = lightGrass;
    }

    if (matchingNeighbors <= 1)
    {
        fragmentColor = vec4(replacementColor, center.a);
    }
    else
    {
        fragmentColor = center;
    }
}