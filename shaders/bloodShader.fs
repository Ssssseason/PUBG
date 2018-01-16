#version 330 core
out vec4 FragColor;

in vec3 FragPos;

void main() {
    vec3 color = vec3(0.9, 0.0, 0.0);
    FragColor = vec4(color, 1.0f);
}
