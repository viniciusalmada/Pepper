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
uniform sampler2D u_texture;

void main()
{
  out_color = texture(u_texture, v_tex_coords * 10.0) * u_color;
}