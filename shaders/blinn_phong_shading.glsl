#version 330 core

vec3 ambient_light(vec3 obj_color, vec3 light_color) {
    return obj_color * light_color;
}

vec3 diffuse_light(vec3 obj_color, vec3 light_color, vec4 normal, vec4 light_direction) {
    return obj_color * light_color * max( dot(normal, light_direction), 0.0);
}

vec3 specular_light(vec3 obj_color, float shininess, vec3 light_color, vec4 normal, vec4 light_direction, vec4 view_direction) {
    vec4 h = normalize( light_direction + view_direction );
    return obj_color * light_color * max( pow( dot( h, normal ), shininess ), 0.0);
}
