#version 330 core

in float vertex_height;      // Height from Vertex Shader
in vec2 frag_texcoord;       // Texture coordinate from Vertex Shader
out vec4 frag_color;         // Output fragment color

uniform vec3 color1;         // Blending start color
uniform vec3 color2;         // Blending end color
uniform vec3 solid_color;    // Solid color

// 1 = terrain blending, 0 = solid color
uniform int use_height_blending;

void main() {
    if (use_height_blending == 1) {
        vec3 blended_color = mix(color1, color2, vertex_height);
        frag_color = vec4(blended_color, 1.0);
    } else {
        frag_color = vec4(solid_color, 1.0);
    }
}
