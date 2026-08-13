#version 330 core

in vec3 worldPosition;

out vec4 fragmentColor;


float randomValue(ivec2 p)
{
    uint x = uint(p.x);
    uint z = uint(p.y);

    uint hash = x * 374761393u + z * 668265263u;

    hash = (hash ^ (hash >> 13u)) * 1274126177u;

    hash ^= hash >> 16u;

    return float(hash) / 4294967295.0;
}

float smoothCurve(float t)
{
    return t * t * (3.0 - 2.0 * t);
}

float sampleNoise(vec2 position)
{
    ivec2 base = ivec2(floor(position));

    ivec2 next = base + ivec2(1, 1);

    float v00 = randomValue(ivec2(base.x, base.y));
    float v10 = randomValue(ivec2(next.x, base.y));
    float v01 = randomValue(ivec2(base.x, next.y));
    float v11 = randomValue(ivec2(next.x, next.y));
    
    vec2 fraction = fract(position);

    vec2 smoothFraction = fraction * fraction * (3.0 - 2.0 * fraction);

    float top = mix(v00, v10, smoothFraction.x);
    float bottom = mix(v01, v11, smoothFraction.x);

    return mix(top, bottom, smoothFraction.y);
}

float sampleGrassField(vec2 position, vec2 offset)
{
    float value = 0.0f;

    value += sampleNoise(position * 0.33 + offset) * 0.8;

    value += sampleNoise(position * 0.5 + offset) * 0.2;

    return value;
}

void main()
{
    vec2 position = worldPosition.xz;

    float dark = sampleGrassField(position, vec2(0.0, 0.0));

    float medium = sampleGrassField(position, vec2(37.0, 71.0));// + 0.08;

    float light = sampleGrassField(position, vec2(113.0, 191.0));

    vec3 grassColor;

    if (dark > medium && dark > light)
    {
        grassColor = vec3(0.18, 0.32, 0.12);
    }
    else if (medium > light)
    {
        grassColor = vec3(0.28, 0.45, 0.18);
    }
    else
    {
        grassColor = vec3(0.42, 0.58, 0.24);
    }

    fragmentColor = vec4(grassColor, 1.0);
}