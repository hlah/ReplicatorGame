#version 330 core

#define DIRECTIONAL     1u
#define POINT           2u
#define SPOTLIGHT       4u

vec3 ambient_light(vec3 obj_color, vec3 light_color);
vec3 diffuse_light(vec3 obj_color, vec3 light_color, vec4 normal, vec4 light_direction);
vec3 specular_light(vec3 obj_color, float shininess, vec3 light_color, vec4 normal, vec4 light_direction, vec4 view_direction);


struct light {
    uint type;

    vec4 position;
    vec4 direction;

    vec3 color;

    float outer_angle;
    float inner_angle;
};

uniform mat4 view_transform;

uniform light lights[16];
uniform uint light_count;

vec4 apply_lights( vec3 ambient_color, vec3 diffuse_color, vec3 specular_color, float shininess, vec4 normal, vec4 position ) {
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_pos = inverse(view_transform) * vec4(0.0, 0.0, 0.0, 1.0);
    vec4 view_direction = normalize( camera_pos - position );
    for( uint i=0u; i<light_count; i++ ) {
        if( (lights[i].type & DIRECTIONAL) > 0u ) {
            vec4 light_direction = -lights[i].direction;
            color += vec4(ambient_light(ambient_color, lights[i].color), 0.0);
            color += vec4(diffuse_light(diffuse_color, lights[i].color, normal, light_direction), 0.0);
            color += vec4(specular_light(specular_color, shininess, lights[i].color, normal, light_direction, view_direction), 0.0);
        }
        if( (lights[i].type & POINT) > 0u ) {
            vec4 light_direction = normalize(lights[i].position - position);
            color += vec4(ambient_light(ambient_color, lights[i].color), 0.0);
            color += vec4(diffuse_light(diffuse_color, lights[i].color, normal, light_direction), 0.0);
            color += vec4(specular_light(specular_color, shininess, lights[i].color, normal, light_direction, view_direction), 0.0);
        }
        if( (lights[i].type & SPOTLIGHT) > 0u ) {
            vec4 light_direction = normalize(lights[i].position - position);
            float theta = dot(light_direction, normalize(-lights[i].direction));
            float intensity = clamp( (theta-lights[i].outer_angle) / (lights[i].inner_angle - lights[i].outer_angle), 0.0, 1.0); 
            color += vec4(ambient_light(ambient_color, lights[i].color), 0.0);
            color += intensity*vec4(diffuse_light(diffuse_color, lights[i].color, normal, light_direction), 0.0);
            color += intensity*vec4(specular_light(specular_color, shininess, lights[i].color, normal, light_direction, view_direction), 0.0);
        }
    }
    return color;
}
