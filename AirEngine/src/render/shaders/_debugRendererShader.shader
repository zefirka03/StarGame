R"(
~~vertex~~
#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 f_color;

uniform mat4 proj;

void main() {
    gl_Position = proj * vec4(position, 1.0);
    f_color = color;
}

~~fragment~~
#version 430 core

uniform sampler2D tex;
out vec4 out_color;

in vec4 f_color;

void main() {
    out_color = f_color;
}
)"