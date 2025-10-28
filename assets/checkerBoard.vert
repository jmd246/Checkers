#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aBaseColor;
layout (location = 2) in vec2 offset;
layout (location = 3) in vec3 aTileColor;

out vec3 vColor;

uniform mat4 proj;
uniform mat4 model;

uniform vec2 tileSize;

void main() {
    // Scale the quad by the tile size before offset
    vec2 scaledPos = aPos * tileSize;
    vec4 worldPos = model * vec4(scaledPos + offset, 0.0, 1.0);

    gl_Position = proj * worldPos;
    vColor = aTileColor;
}
