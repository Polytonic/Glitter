#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};
const int NR_LIGHTS = 5;
uniform Light lights[NR_LIGHTS];

uniform vec3 directionalLightInDir;
uniform vec3 directionalLightColor;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-1.0 * directionalLightInDir);
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.005;
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    /*
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }    
    }
    shadow /= 9.0;
    */

    float pcfDepth = texture(shadowMap, projCoords.xy).r;
    shadow = currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec3 FragPos = fs_in.FragPos;
    vec3 Normal = normalize(fs_in.Normal);
    vec3 Diffuse = texture(diffuseTexture, fs_in.TexCoords).rgb;
    
    // then calculate lighting as usual
    vec3 lighting = vec3(0.0, 0.0, 0.0);
    lighting += Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].Color * spec * Diffuse;
        // attenuation
        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }

    {
        vec3 lightDir = normalize(-1.0 * directionalLightInDir);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * directionalLightColor;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = directionalLightColor * spec * Diffuse;
	float directional_shadow = 1.0 - ShadowCalculation(fs_in.FragPosLightSpace);
        lighting += directional_shadow * diffuse;
	lighting += directional_shadow * specular;
    }

    FragColor = vec4(lighting, 1.0);
}
