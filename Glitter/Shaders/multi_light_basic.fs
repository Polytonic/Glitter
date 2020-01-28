#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};
const int NR_LIGHTS = 16;
uniform Light lights[NR_LIGHTS];

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 FragPos = fs_in.FragPos;
    vec3 Normal = normalize(fs_in.Normal);
    vec3 Diffuse = texture(diffuseTexture, fs_in.TexCoords).rgb;
    
    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
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
    FragColor = vec4(lighting, 1.0);
}
