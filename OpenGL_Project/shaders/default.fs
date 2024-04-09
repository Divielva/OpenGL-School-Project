#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
    bool hasNormalMap;
    bool hasSpecularMap;
    vec3 albedo;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;

    Light light;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    Light light;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    Light light;

    sampler2D shadowMap;
    vec4 lightSpaceMatrix;
};

#define NR_POINT_LIGHTS 4

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform float gammaCorrection;
uniform sampler2D shadowMap;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalcuation(vec4 fragPosLightSpace, float bias);
vec3 GammaCorrect(vec3 color);

void main() {
    // properties
    vec3 normal = normalize(fs_in.Normal);
    if(material.hasNormalMap) {
        normal = texture(material.normal, fs_in.TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0); // this normal is in tangent space
    }
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, normal, viewDir);
    // phase 2: point lights
    // for(int i = 0; i < NR_POINT_LIGHTS; i++) result += CalcPointLight(pointLights[i], normal, fs_in.TangentFragPos, viewDir);    
    // // phase 3: spot light
    // result += CalcSpotLight(spotLight, normal, fs_in.FragPos, viewDir);

    FragColor = vec4(GammaCorrect(result), 1.0);
}

vec3 GetSpecular(Light light, float spec) {
    if(material.hasSpecularMap)
        return light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    return vec3(spec);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = GetSpecular(light.light, spec);
    float dotProduct = max(dot(normal, lightDir), 0.0);
    float bias = 0.005 * tan(acos(dotProduct));
    float shadow = ShadowCalcuation(fs_in.FragPosLightSpace, clamp(bias, 0, 0.01));
    return (ambient + (diffuse + specular) * (1 - shadow));
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(fs_in.TBN * light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = GetSpecular(light.light, spec);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(fs_in.TBN * light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = GetSpecular(light.light, spec);
    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

vec3 GammaCorrect(vec3 color) {
    return pow(color, vec3(1.0 / gammaCorrection));
}

float ShadowCalcuation(vec4 fragPosLightSpace, float bias) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if(currentDepth > 1.0)
        return 0.0;
    // check whether current frag pos is in shadow
    float shadow = 0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
}