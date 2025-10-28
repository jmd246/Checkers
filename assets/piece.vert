#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 proj;
uniform mat4 model;
uniform vec2 tileSize;

out vec2 FragPos;

void main() {
    // Scale the quad by the tile size before offset
    vec4 worldPos = model * vec4(aPos, 0.0, 1.0);

    gl_Position = proj * worldPos;
}
