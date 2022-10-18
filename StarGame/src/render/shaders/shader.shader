~~vertex~~
#version 430 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 size;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 origin;
layout(location = 4) in vec2 scale;
layout(location = 5) in float rotation;
layout(location = 6) in vec4 texRect;

out vec2 _position;
out vec2 _size;
out vec4 _color;
out vec2 _origin;
out vec2 _scale;
out float _rotation;
out vec4 _texRect;

void main() {
    gl_Position = vec4(0, 0, 1.0, 1.0);
    _position = position;
    _size = size;
    _color = color;
    //_rotation = rotation;
    //_origin = origin;
    //_texRect = texRect;
    //_scale = scale;

    _rotation = 0;
    _origin = vec2(0);
    //_texRect = texRect;
    _scale = vec2(1);
}

~~geometry~~
#version 430 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 _position[];
in vec2 _size[];
in vec4 _color[];
in vec2 _origin[];
in vec2 _scale[];
in float _rotation[];
in vec4 _texRect[];

uniform mat4 proj;

out vec2 f_texture_coord;
out vec4 f_color;

void main() {
    vec2 ps;
    mat2 rot_mat = mat2(cos(_rotation[0]), -sin(_rotation[0]),
        sin(_rotation[0]), cos(_rotation[0]));

    ps = gl_in[0].gl_Position.xy - _origin[0] + vec2(0.0, 0.0);
    f_texture_coord = vec2(_texRect[0][0], _texRect[0][1]);
    f_color = _color[0];
    gl_Position = proj * vec4(_position[0] + rot_mat * (_scale[0] * ps), 1, 1);
    EmitVertex();

    ps = gl_in[0].gl_Position.xy - _origin[0] + vec2(_size[0].x, 0.0);
    f_texture_coord = vec2(_texRect[0][0] + _texRect[0][2], _texRect[0][1]);
    f_color = _color[0];
    gl_Position = proj * vec4(_position[0] + rot_mat * (_scale[0] * ps), 1, 1);
    EmitVertex();

    ps = gl_in[0].gl_Position.xy - _origin[0] + vec2(0.0, _size[0].y);
    f_texture_coord = vec2(_texRect[0][0], _texRect[0][1] + _texRect[0][3]);
    f_color = _color[0];
    gl_Position = proj * vec4(_position[0] + rot_mat * (_scale[0] * ps), 1, 1);
    EmitVertex();

    ps = gl_in[0].gl_Position.xy - _origin[0] + vec2(_size[0].x, _size[0].y);
    f_texture_coord = vec2(_texRect[0][0] + _texRect[0][2], _texRect[0][1] + _texRect[0][3]);
    f_color = _color[0];
    gl_Position = proj * vec4(_position[0] + rot_mat * (_scale[0] * ps), 1, 1);
    EmitVertex();

    EndPrimitive();
}

~~fragment~~
#version 430 core

uniform sampler2D tex;
out vec4 out_color;
in vec2 f_texture_coord;
in vec4 f_color;

void main() {
    out_color = f_color;
   // color = texture(tex, texture_coord);
}