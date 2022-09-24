//##vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_coords;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

out vec2 v_tex_coords;

void main()
{
  gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
  v_tex_coords = in_tex_coords;
}

//##fragment
#version 330 core

in vec2 v_tex_coords;

out vec4 out_color;

uniform vec4 u_color;
uniform float u_pixel_fac;
uniform sampler2D u_texture;

void main()
{
  vec2 grid_uv = round(v_tex_coords * u_pixel_fac) / u_pixel_fac;

  out_color = textureLod(u_texture, grid_uv, 0) * u_color;
}