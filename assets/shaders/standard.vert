#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 uv;

void main()
{
    vec4 worldPos = model * vec4(position, 1.0);

    worldPosition = worldPos.xyz;

    worldNormal = normalize(mat3(transpose(inverse(model))) * normal);

    uv = texCoord;

    gl_Position = projection * view * worldPos;
}