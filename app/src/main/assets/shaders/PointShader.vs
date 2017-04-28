#version 300 es
uniform mat4 viewProjection;

in vec2 xy;
in float z;

out vec3 finalPos;
void main() {
    finalPos = vec3(xy.xy, z);
    gl_Position = viewProjection * vec4(finalPos, 1);
}