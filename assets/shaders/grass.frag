#version 330 core

in vec3 vertexColor;
in vec2 uv;

out vec4 fragmentColor;

uniform sampler2D grassTexture;

void main()
{
    vec4 texel = texture(grassTexture, uv);

    if(texel.a < 0.5)
    {
        discard;
    }

    fragmentColor = vec4(vertexColor, 1.0);

}