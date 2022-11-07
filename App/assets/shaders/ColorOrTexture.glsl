//##vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_tex_coords;
layout(location = 3) in float in_texture_id;

uniform mat4 u_view_projection;

out vec2 v_tex_coords;
out vec4 v_color;
flat out float v_texture_id;

void main()
{
  gl_Position = u_view_projection * vec4(in_position, 1.0);
  v_tex_coords = in_tex_coords;
  v_color = in_color;
  v_texture_id = in_texture_id;
}

//##fragment
#version 330 core

in vec2 v_tex_coords;
in vec4 v_color;
flat in float v_texture_id;

out vec4 out_color;

uniform sampler2D u_textures[32];

void main()
{
  out_color = texture(u_textures[int(v_texture_id)], v_tex_coords) * v_color;
}