#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

out vec2 texCoord;

uniform mat4 u_Transform;

void main()
{
	gl_Position = u_Transform * position;
	texCoord = texCoords;
}