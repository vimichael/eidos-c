@header #include "cglm/cglm.h"
@ctype mat4 mat4

@vs vertex_shader

in vec3 pos;
in vec3 color;

out vec3 out_color;

layout(binding=0) uniform triangle_params {
  mat4 mvp;
};

void main() {
  gl_Position = mvp * vec4(pos, 1.0);
  out_color = color;
}

@end

@fs fragment_shader

in vec3 out_color;

out vec4 frag_color;

void main() {
  frag_color = vec4(out_color, 1.0);
}

@end

@program triangle vertex_shader fragment_shader
