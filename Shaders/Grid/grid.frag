#version 450

layout (location = 0) in vec3 v_WorldPosition;

layout (location = 0) out vec4 FragColor;


struct GridLOD
{
    float cellSize;
    float lineThickness;
    float distanceFallOffFactor;
    float baseAlpha;
};

const uint LOD_COUNT = 2;
const GridLOD[LOD_COUNT] LODS = {   { 100., 4., 0.006, 0.15 },
                                    { 10.,  3., 0.007, 0.1  }};
                                    //{ 1. ,  2., 0.02,  0.05 } };


void main()
{
    float dxx = abs(dFdx(v_WorldPosition.x));
    float dyx = abs(dFdy(v_WorldPosition.x));

    float dxz = abs(dFdx(v_WorldPosition.z));
    float dyz = abs(dFdy(v_WorldPosition.z));


    float finalAlpha = 0.;    
    for (uint i = 0; i<LOD_COUNT; i++)
    {
        float x = (mod(v_WorldPosition.x, LODS[i].cellSize)) / (max(dxx,dyx));
        float z = (mod(v_WorldPosition.z, LODS[i].cellSize)) / (max(dxz,dyz));

        float ax = x;
        ax = (1/LODS[i].lineThickness) * ax;
        ax = clamp(ax, 0., 1.);
        ax = 2.*(ax-0.5);
        ax = 1. - abs(ax);
        float az = z;
        az = (1/LODS[i].lineThickness) * az;
        az = clamp(az, 0., 1.);
        az = 2.*(az-0.5);
        az = 1. - abs(az);

        float a = max(ax, az);

        float distanceFactor = 1. - clamp(LODS[i].distanceFallOffFactor * sqrt(v_WorldPosition.x*v_WorldPosition.x + v_WorldPosition.z*v_WorldPosition.z), 0., 1.);
        a *= distanceFactor;

        a *= LODS[i].baseAlpha;

        finalAlpha = max(a, finalAlpha);
    }

    FragColor = vec4(vec3(0.6), finalAlpha);
}