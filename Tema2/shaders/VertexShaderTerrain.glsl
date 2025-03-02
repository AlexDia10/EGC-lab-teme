#version 330 core

layout(location = 0) in vec3 in_position;  // Vertex position
layout(location = 1) in vec3 in_normal;    // Vertex normal

out vec3 frag_position;   // Pass position to Fragment Shader
out vec2 frag_texcoord;   // Pass texture coordinate to Fragment Shader
out float vertex_height;  // Pass normalized height to Fragment Shader

uniform mat4 Model;       // Model matrix
uniform mat4 View;        // View matrix
uniform mat4 Projection;  // Projection matrix

// Simple random function for noise generation
float random(in vec2 st) {  
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Noise function
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

// Fractal Noise (Octaves) to add more details and smooth out the noise
float fractalNoise(vec2 st) {
    float amplitude = 1.0;
    float frequency = 1.0;
    float maxAmplitude = 0.0;
    float total = 0.0;
    
    for (int i = 0; i < 5; i++) {
        total += noise(st * frequency) * amplitude;
        maxAmplitude += amplitude;
        amplitude *= 0.5;  // Reduce amplitude with each octave
        frequency *= 2.0;  // Increase frequency with each octave
    }

    // Normalize the result to the range [0, 1]
    return (total / maxAmplitude + 1.0) / 2.0;
}

void main() {
    vec3 position = in_position;

    // Noise parameters
    float frequency = 0.1;
    float amplitude = 2.0;

    // Apply fractal noise for smoother terrain
    float raw_noise = fractalNoise(position.xz * frequency); // Using fractal noise for smoother result
    position.y += amplitude * raw_noise - 1.0;

    // Normalize noise to the range [0, 1]
    vertex_height = raw_noise;

    // Pass values to the fragment shader
    frag_position = position;
    frag_texcoord = position.xz;

    // Final transformed position
    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
