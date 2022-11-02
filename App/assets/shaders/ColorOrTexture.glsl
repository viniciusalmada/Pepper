//##vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_tex_coords;

uniform mat4 u_view_projection;

out vec2 v_tex_coords;
out vec4 v_color;

void main()
{
  gl_Position = u_view_projection * vec4(in_position, 1.0);
  v_tex_coords = in_tex_coords;
  v_color = in_color;
}

//##fragment
#version 330 core

in vec2 v_tex_coords;
in vec4 v_color;

out vec4 out_color;

uniform float u_tiling_factor;
uniform sampler2D u_texture;

void main()
{
  // out_color = texture(u_texture, v_tex_coords * u_tiling_factor) * v_color;
  out_color = v_color;
}