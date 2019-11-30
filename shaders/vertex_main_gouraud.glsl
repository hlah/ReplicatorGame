#version 330 core

layout (location = 0) in vec4 vertice_in;
layout (location = 1) in vec4 color_in;
layout (location = 2) in vec4 normal_in;
layout (location = 3) in vec2 texcoord_in;

out vec4 color_f;

uniform mat4 model_transform;
uniform mat4 projection_transform;
uniform mat4 view_transform;

uniform struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D ambient_textures[4];
    uint ambient_texture_count;
    sampler2D diffuse_textures[4];
    uint diffuse_texture_count;
    sampler2D specular_textures[4];
    uint specular_texture_count;
} material;

vec4 apply_lights( vec3 ambient_color, vec3 diffuse_color, vec3 specular_color, float shininess, vec4 normal, vec4 position );

void main() {
    position_f = model_transform * vertice_in;
    gl_Position = projection_transform * view_transform * position_f;
    normal_f = vec4(normalize((inverse(transpose(model_transform))*normal_in).xyz), 0.0);
    texcoords_f = texcoord_in;

    vec3 ambient_color = material.ambient;
    vec3 diffuse_color = material.diffuse;
    vec3 specular_color = material.specular;

    for( uint i=0u; i<material.ambient_texture_count; i++ ) {
        vec3 tex_color = texture( material.ambient_textures[i], texcoords_f ).rgb;
        ambient_color += tex_color;
    }
    for( uint i=0u; i<material.diffuse_texture_count; i++ ) {
        vec3 tex_color = texture( material.diffuse_textures[i], texcoords_f ).rgb;
        diffuse_color += tex_color;
    }
    for( uint i=0u; i<material.specular_texture_count; i++ ) {
        vec3 tex_color = texture( material.specular_textures[i], texcoords_f ).rgb;
        specular_color += tex_color;
    }

    color_f = apply_lights( ambient_color, diffuse_color, specular_color, material.shininess, normal_f, position_f );
}
