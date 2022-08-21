//##vertex
#version 330 core

layout(location = 0) in vec3 in_position;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

void main() { gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0); }

//##fragment
#version 330 core

out vec4 out_color;

uniform vec3 u_color;

void main() { out_color = vec4(u_color, 1.0); }