#pragma once

#ifndef SHADER_CODE_GLSL_TO_STRING
#define SHADER_CODE_GLSL_TO_STRING(version, shader)  "#version " #version " core \n" #shader  
#endif

static const char *face_idx_vertex_shader_source = SHADER_CODE_GLSL_TO_STRING(330,
    layout(location = 0) in vec3 a_pos;
    layout(location = 1) in vec3 a_id; // a id of the face((r * 256^3 + g * 256^2 + b) * 256 - 1)

    flat out vec3 vs_id; //flat/smooth/noperspective

    uniform mat4 model;
    uniform mat4 normal_matrix;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        vs_id = a_id;

        // info for fragment shader
        vec3 vs_frag_position = vec3(model * vec4(a_pos, 1.0));
    
        // final coordinates
        gl_Position = projection * view * vec4(vs_frag_position, 1.0);
    }
);