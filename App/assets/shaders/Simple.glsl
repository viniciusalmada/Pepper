//##vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

out vec4 v_color;

void main()
{
  gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
  v_color = in_color;
}

//##fragment
#version 330 core

in vec4 v_color;
out vec4 out_color;

void main() { out_color = v_color; }