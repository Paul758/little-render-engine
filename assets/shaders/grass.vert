#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec2 localPosition;

out vec3 vertexColor;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform float spriteWidth;
uniform float spriteHeight;

void main() 
{
    vec3 worldCenter = vec3(model * vec4(position, 1.0));

    vec3 worldPosition = worldCenter + cameraRight * localPosition.x * spriteWidth + cameraUp * localPosition.y * spriteHeight;


    gl_Position = projection * view * vec4(worldPosition, 1.0);

    vertexColor = color;
    uv = texCoord;
}