R"(
~~vertex~~
#version 100
precision highp float;
attribute vec3 position;
attribute vec4 color;
attribute vec2 _textureCoord;
varying vec4 vcolor;
varying vec2 vtextureCoord;

uniform mat4 proj;

void main() {
  gl_Position = proj * vec4(position, 1.0);
  vcolor = color;
  vtextureCoord = _textureCoord;
}

~~fragment~~
#version 100
precision highp float;
varying vec4 vcolor;
varying vec2 vtextureCoord;

uniform sampler2D tex;

void main() {
  gl_FragColor = vcolor * texture2D(tex, vtextureCoord);
}

)"