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

 layout (std140, binding = 4) uniform LightSpace //TEMP
{
    mat4 Matrix;
} ub_LightSpace;


layout (location = 0) in vec3 v_WorldPosition;
layout (location = 1) in vec3 v_WorldNormal;
layout (location = 2) in vec3 v_ViewPosition;
layout (location = 3) in vec3 v_ViewNormal;

layout (location = 0) out vec4 FragColor;

layout(binding = 10) uniform sampler2D ShadowMap;


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
float ShadowCalculation(vec4 fragPosLightSpace, vec3 direction);



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
        ComputeSpotlight(ub_Lighting.SpotLights[i]);
    for(int i = 0; i < ub_Lighting.LightsCounts.z; i++)
        ComputeDirectionallight(ub_Lighting.DirLights[i]);

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

    vec4 fragPosLightSpace = ub_LightSpace.Matrix * vec4(v_WorldPosition, 1.0);
    float shadow = 1. - ShadowCalculation(fragPosLightSpace, lightDirection);

    cumulativeLightComposition.Ambient += ambient;
    cumulativeLightComposition.Diffuse += diffuse * shadow;
    cumulativeLightComposition.Specular += specular * shadow;
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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 direction)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(ShadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    //float bias = max(0.05 * (1.0 - dot(normal, direction)), 0.0005);
    float bias = 0.0003;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  