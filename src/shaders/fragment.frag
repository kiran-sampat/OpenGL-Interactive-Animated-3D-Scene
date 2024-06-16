#version 330 core

in vec2 tex;
uniform sampler2D Texture;

in vec3 nor;
in vec3 FragPos;

struct LIGHT
{
    vec3 lightDirection;
    vec3 lightColor;
    vec3 lightPos;
};

uniform LIGHT light_1;
uniform LIGHT light_2;

uniform vec3 camPos;

out vec4 fragColour;

float calculate_directional_illumination(LIGHT light);
float calculate_positional_illumination(LIGHT light);
float calculate_spot_illumination(LIGHT light);
float calculate_attenuation(LIGHT light);

void main()
{
    float phong_moving = calculate_spot_illumination(light_1);
    float phong_stationary = calculate_positional_illumination(light_2);
    // float phong = calculate_directional_illumination();
    float light_combined = phong_moving + phong_stationary;
    vec3 col = texture(Texture, tex).rgb * light_1.lightColor;

    fragColour = vec4(light_combined * col, 1.f);
}

float calculate_directional_illumination(LIGHT light)
{
    // ambient
    float ambient = 0.1f;

    // diffuse
    vec3 Nnor = normalize(nor);
    vec3 Nto_light = normalize(-light.lightDirection);
    float diffuse = max(dot(Nnor, Nto_light), 0.0);

    // specular
    vec3 Nfrom_light = normalize(light.lightDirection);
    vec3 NrefLight = reflect(Nfrom_light, Nnor);
    vec3 camDirection = camPos - FragPos;
    vec3 NcamDirection = normalize(camDirection);
    int shininess = 64;
    float specular = pow(max(dot(NcamDirection, NrefLight), 0.0), shininess);

    // calculate phong
    float phong = ambient + diffuse + specular;
    return phong;
}

float calculate_positional_illumination(LIGHT light)
{
    // ambient
    float ambient = 0.01f;

    // diffuse
    vec3 Nnor = normalize(nor);
    vec3 Nto_light = normalize(light.lightPos - FragPos);
    float diffuse = max(dot(Nnor, Nto_light), 0.0);

    // specular
    vec3 Nfrom_light = -Nto_light;
    vec3 NrefLight = reflect(Nfrom_light, Nnor);
    vec3 camDirection = camPos - FragPos;
    vec3 NcamDirection = normalize(camDirection);
    int shininess = 64;
    float specular = pow(max(dot(NcamDirection, NrefLight), 0.0), shininess);

    // attenuation
    float attenuation = calculate_attenuation(light);

    // calculate phong
    float phong = (ambient + diffuse + specular) * attenuation;
    return phong;
}

float calculate_spot_illumination(LIGHT light)
{
    // ambient
    float ambient = 0.01f;

    // diffuse
    vec3 Nnor = normalize(nor);
    vec3 Nto_light = normalize(light.lightPos - FragPos);
    float diffuse = max(dot(Nnor, Nto_light), 0.0);

    // specular
    vec3 Nfrom_light = -Nto_light;
    vec3 NrefLight = reflect(Nfrom_light, Nnor);
    vec3 camDirection = camPos - FragPos;
    vec3 NcamDirection = normalize(camDirection);
    int shininess = 16;
    float specular = pow(max(dot(NcamDirection, NrefLight), 0.0), shininess);

    // attenuation
    float attenuation = calculate_attenuation(light);

    // the cutoff angle that specifies the spotlight's radius.
    // Everything outside this angle is not lit by the spotlight.

    // calculate phi and theta
    float cut_off_angle = radians(15.0);
    float phi = cos(cut_off_angle);

    //vec3 NSpotDir = normalize(light.lightDirection);
    //float theta = dot(Nfrom_light, NSpotDir);
    vec3 NSpotDir = normalize(-light.lightDirection);
    float theta = dot(Nto_light, NSpotDir);

    // calculate phong
    float phong;

    if (theta > phi)
        phong = (ambient + diffuse + specular) * attenuation;
    else
        phong = ambient * attenuation;

    return phong;
}

float calculate_attenuation(LIGHT light)
{
    //calculate attenuation;
    float att_constant = 1.5f;
    float att_linear = 0.05f;
    float att_quadratic = 0.02f;

    float distance = length(light.lightPos - FragPos);
    float attenuation = 1.0 / (att_constant + (att_linear * distance) + (att_quadratic * (distance * distance)));

    return attenuation;
}
