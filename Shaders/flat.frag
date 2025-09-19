#version 450


#define MAX_POINT_LIGHTS 64
#define MAX_SPOT_LIGHTS  64
#define MAX_DIR_LIGHTS   8


struct Pointlight 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuationParameters;
};

struct Directionallight 
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight 
{
    vec3 position;
    vec3 direction;
    vec2 cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuationParameters;
};

layout (std140, binding = 1) uniform Lighting
{
    vec3 LightsCounts;
    Pointlight[MAX_POINT_LIGHTS] PointLights;
    Spotlight[MAX_SPOT_LIGHTS] SpotLights;
    Directionallight[MAX_DIR_LIGHTS] DirLights;
} ub_Lighting;

layout (std140, binding = 2) uniform Material
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
 } ub_Material;


layout (location = 0) in vec3 v_WorldPosition;
layout (location = 1) in vec3 v_WorldNormal;
layout (location = 2) in vec3 v_ViewPosition;
layout (location = 3) in vec3 v_ViewNormal;

layout (location = 0) out vec4 FragColor;



struct LightComposition
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};
LightComposition cumulativeLightComposition;
vec3 viewDirection;
vec3 normal;


void ComputeDirectionallight(Directionallight light);
void ComputePointlight(Pointlight light);
void ComputeSpotlight(Spotlight light);



void main()
{
    normal = normalize(v_ViewNormal); // renormilized normal to correct artifact caused by MSAA (when the pixel center is outside of a triangle but one sample is inside the triangle the normal can be extrapolated and thus not normilized anymore)
    viewDirection = normalize(-v_ViewPosition);

    cumulativeLightComposition.Ambient = vec3(0);
    cumulativeLightComposition.Diffuse = vec3(0);
    cumulativeLightComposition.Specular = vec3(0);

    for(int i = 0; i < ub_Lighting.LightsCounts.x; i++)
        ComputePointlight(ub_Lighting.PointLights[i]);
    for(int i = 0; i < ub_Lighting.LightsCounts.y; i++)
        ComputeDirectionallight(ub_Lighting.DirLights[i]);
    for(int i = 0; i < ub_Lighting.LightsCounts.z; i++)
        ComputeSpotlight(ub_Lighting.SpotLights[i]);

    vec3 result = cumulativeLightComposition.Ambient * ub_Material.Ambient  + cumulativeLightComposition.Diffuse * ub_Material.Diffuse + cumulativeLightComposition.Specular * ub_Material.Specular;
    FragColor = vec4(result, 1.0);
}


void ComputePointlight(Pointlight light)
{

    vec3 lightDirection = normalize(light.position - v_ViewPosition);
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);   


    // basic phong :
    // vec3 reflectedDirection = reflect(-lightDirection, normal); 
    // float specularIntensity = pow(max(dot(viewDirection , reflectedDirection), 0.0), ub_Material.Shininess);

    // blinn-phong :
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(dot(normal , halfwayDirection), 0.0), ub_Material.Shininess);


    float distance = length(light.position - v_WorldPosition);
    float attenuation = 1.0 / (light.attenuationParameters.x + light.attenuationParameters.y * distance + light.attenuationParameters.z * (distance * distance));

    vec3 ambient = light.ambient * attenuation;
    vec3 diffuse = light.diffuse * diffuseIntensity * attenuation;
    vec3 specular = light.specular * specularIntensity * attenuation;

    cumulativeLightComposition.Ambient += ambient;
    cumulativeLightComposition.Diffuse += diffuse;
    cumulativeLightComposition.Specular += specular;
}

void ComputeDirectionallight(Directionallight light)
{

    vec3 lightDirection = normalize(-light.direction);
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);

    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(dot(normal , halfwayDirection), 0.0), ub_Material.Shininess);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diffuseIntensity;
    vec3 specular = light.specular * specularIntensity;

    cumulativeLightComposition.Ambient += ambient;
    cumulativeLightComposition.Diffuse += diffuse;
    cumulativeLightComposition.Specular += specular;
}

void ComputeSpotlight(Spotlight light)
{
    vec3 lightDirection = normalize(light.position - v_WorldPosition);
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);

    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float specularIntensity = pow(max(dot(normal , halfwayDirection), 0.0), ub_Material.Shininess);

    float distance = length(light.position - v_WorldPosition);
    float attenuation = 1.0 / (light.attenuationParameters.x + light.attenuationParameters.y * distance + light.attenuationParameters.z * (distance * distance));

    float theta = dot(lightDirection, normalize(-light.direction));
    float t = clamp( (theta-light.cutoff.y) / (light.cutoff.x-light.cutoff.y), 0.0, 1.0);
    float fallOff = t*t;

    vec3 ambient = light.ambient * attenuation * fallOff;
    vec3 diffuse = light.diffuse * diffuseIntensity * attenuation * fallOff;
    vec3 specular = light.specular * specularIntensity * attenuation * fallOff;

    cumulativeLightComposition.Ambient += ambient;
    cumulativeLightComposition.Diffuse += diffuse;
    cumulativeLightComposition.Specular += specular;
}