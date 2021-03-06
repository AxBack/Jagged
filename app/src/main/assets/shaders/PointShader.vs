#version 300 es
uniform mat4 viewProjection;

vec3 light = vec3(250.0f, 50.0f, -100.0f);
vec4 top = vec4(66.0f/255.0f, 134.0f/255.0f, 244.0f/255.0f, 1);
vec4 bottom = vec4(1.0f/255.0f, 36.0f/255.0f, 91.0f/255.0f, 1);

in vec2 xy;
in float z;

out vec3 finalPos;
out vec3 toLightNormal;
out vec4 color;

void main() {
    float zFactor = smoothstep(95.0f, 1.0f, z);
    color = mix(bottom, top, zFactor);
    finalPos = vec3(xy.xy, z);
    toLightNormal = normalize(light - finalPos);
    gl_Position = viewProjection * vec4(finalPos, 1);
}