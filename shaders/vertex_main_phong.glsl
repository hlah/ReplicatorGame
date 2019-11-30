#version 330 core

layout (location = 0) in vec4 vertice_in;
layout (location = 1) in vec4 color_in;
layout (location = 2) in vec4 normal_in;
layout (location = 3) in vec2 texcoord_in;

out vec4 color_f;
out vec4 normal_f;
out vec4 position_f;
out vec2 texcoords_f;

uniform mat4 model_transform;
uniform mat4 projection_transform;
uniform mat4 view_transform;

void main() {
    position_f = model_transform * vertice_in;
    gl_Position = projection_transform * view_transform * position_f;
    color_f = color_in;
    normal_f = vec4(normalize((inverse(transpose(model_transform))*normal_in).xyz), 0.0);
    texcoords_f = texcoord_in;
}
