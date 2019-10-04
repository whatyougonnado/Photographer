#pragma once

#ifndef SHADER_CODE_GLSL_TO_STRING
#define SHADER_CODE_GLSL_TO_STRING(version, shader)  "#version " #version " core \n" #shader  
#endif

static const char *flat_fragment_shader_source = SHADER_CODE_GLSL_TO_STRING(330,

    // output color!
    out vec4 frag_color;

    // From Vertex shader
    flat in vec3 vs_color;

    // ------------------ main ---------------------
    void main()
    {
        frag_color = vec4(vs_color, 1.0);
    }
);