#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;

out vec4 fragmentColor;

//Material
struct Material
{
    vec3 albedoColor;

    bool hasAlbedoTexture;
    sampler2D albedoTexture;
};


//Directional Light
struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};


uniform Material material;
uniform DirectionalLight directionalLight;

uniform vec3 ambientColor;


void main()
{
    // --------------------
    // Material
    // --------------------

    vec3 baseColor = material.albedoColor;

    if (material.hasAlbedoTexture)
    {
        baseColor *= texture(material.albedoTexture, uv).rgb;
    }

    // --------------------
    // Lighting
    // --------------------

    vec3 N = normalize(worldNormal);

    //lightDirection is the direction the light rays travel

    vec3 L = normalize(-directionalLight.direction);

    float diffuseFactor = max(dot(N, L), 0.0);

    vec3 diffuse = baseColor * directionalLight.color * directionalLight.intensity * diffuseFactor;

    vec3 ambient = baseColor * ambientColor;

    // --------------------
    // Final result
    // --------------------

    vec3 finalColor = ambient + diffuse;

    fragmentColor = vec4(finalColor, 1.0);

}