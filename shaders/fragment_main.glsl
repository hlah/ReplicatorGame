#version 330 core

in vec4 color_f;
in vec4 normal_f;
in vec4 position_f;
in vec2 texcoords_f;
out vec4 color;

uniform struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D diffuse_textures[4];
    uint diffuse_texture_count;
    sampler2D specular_textures[4];
    uint specular_texture_count;
} material;

vec4 apply_lights( vec3 ambient_color, vec3 diffuse_color, vec3 specular_color, float shininess, vec4 normal, vec4 position );

void main() {

    vec3 ambient_color = material.ambient;
    vec3 diffuse_color = material.diffuse;
    vec3 specular_color = material.specular;

    for( uint i=0u; i<material.diffuse_texture_count; i++ ) {
        vec3 tex_color = texture( material.diffuse_textures[i], texcoords_f ).rgb;
        //ambient_color += tex_color;
        diffuse_color += tex_color;
    }
    for( uint i=0u; i<material.specular_texture_count; i++ ) {
        vec3 tex_color = texture( material.specular_textures[i], texcoords_f ).rgb;
        specular_color += tex_color;
    }

    color = apply_lights( ambient_color, diffuse_color, specular_color, material.shininess, normal_f, position_f );
}
