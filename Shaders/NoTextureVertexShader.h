#pragma once

#ifndef SHADER_CODE_GLSL_TO_STRING
#define SHADER_CODE_GLSL_TO_STRING(version, shader)  "#version " #version " core \n" #shader  
#endif

static const char *no_texture_vertex_shader_source = SHADER_CODE_GLSL_TO_STRING(330,
    layout(location = 0) in vec3 a_pos;
    layout(location = 1) in vec3 a_normal;

    out vec3 vs_normal;
    out vec3 vs_frag_position;  // in world coordinates

    uniform mat4 model;
    uniform mat4 normal_matrix;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        // info for fragment shader
        vs_frag_position = vec3(model * vec4(a_pos, 1.0));

        // avoid scaling issues. Equivalent to vector transformation
        vs_normal = mat3(normal_matrix) * a_normal;

        // final coordinates
        gl_Position = projection * view * vec4(vs_frag_position, 1.0);
    }
);