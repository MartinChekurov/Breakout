#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 iTexCoord;
out vec2 oTexCoord;
uniform mat4 model;
uniform mat4 projection;
void main()
{
oTexCoord = iTexCoord;
gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
