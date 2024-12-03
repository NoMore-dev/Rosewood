#version 460 core


#define MAX_POINT_LIGHTS 5
#define MAX_SPOT_LIGHTS  5
#define MAX_DIR_LIGHTS   1


struct Pointlight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Directionallight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


in vec3 v_WorldPosition;
in vec3 v_WorldNormal;

out vec4 FragColor;


uniform vec4 u_Color;

uniform int u_PointLightsCount = 0;
uniform int u_DirLightsCount = 0;
uniform int u_SpotLightsCount = 0;

uniform Pointlight[MAX_POINT_LIGHTS] u_Pointlights;
uniform Directionallight[MAX_DIR_LIGHTS] u_Directionallights;
uniform Spotlight[MAX_SPOT_LIGHTS] u_Spotlights;


vec3[3] cumulativeLightComputation;

void ComputeDirectionallight(Directionallight light);
void ComputePointlight(Pointlight light);
void ComputeSpotlight(Spotlight light);


void main()
{
    cumulativeLightComputation[0] = vec3(0);
    cumulativeLightComputation[1] = vec3(0);
    cumulativeLightComputation[2] = vec3(0);

    for(int i = 0; i < u_PointLightsCount; i++)
        ComputePointlight(u_Pointlights[i]);
    for(int i = 0; i < u_DirLightsCount; i++)
        ComputeDirectionallight(u_Directionallights[i]);
    for(int i = 0; i < u_SpotLightsCount; i++)
        ComputeSpotlight(u_Spotlights[i]);

    vec3 result = (cumulativeLightComputation[0] + cumulativeLightComputation[1]) * u_Color.xyz;
    FragColor = vec4(result, 1.0f);
}


void ComputePointlight(Pointlight light){
    vec3 ambient = light.ambient ;
    vec3 norm = v_WorldNormal;
    vec3 lightDir = normalize(light.position - v_WorldPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff ;

    // vec3 viewDir = normalize(viewPos - v_WorldPosition);
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * spec ;

    float distance = length(light.position - v_WorldPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    //specular *= attenuation;

    cumulativeLightComputation[0] += ambient;
    cumulativeLightComputation[1] += diffuse;
    // cumulativeLightComputation[2] += specular;
}

void ComputeDirectionallight(Directionallight light){
    vec3 ambient = light.ambient ;
    vec3 norm = v_WorldNormal;
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff ;

    // vec3 viewDir = normalize(viewPos - v_WorldPosition);
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * spec ;

    cumulativeLightComputation[0] += ambient;
    cumulativeLightComputation[1] += diffuse;
    // cumulativeLightComputation[2] += specular;
}

void ComputeSpotlight(Spotlight light){
    vec3 ambient = light.ambient ;
    vec3 norm = v_WorldNormal;
    vec3 lightDir = normalize(light.position - v_WorldPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff ;

    // vec3 viewDir = normalize(viewPos - v_WorldPosition);
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * spec ;

    float distance = length(light.position - v_WorldPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    // specular *= attenuation;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    // specular *= intensity;

    cumulativeLightComputation[0] += ambient;
    cumulativeLightComputation[1] += diffuse;
    // cumulativeLightComputation[2] += specular;
}