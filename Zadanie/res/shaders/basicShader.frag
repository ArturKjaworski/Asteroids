#version 330

in vec2 texCoord;
out vec4 out_color;

uniform float alpha;
uniform sampler2D tex;

void main()
{
	out_color = texture2D(tex, texCoord) * vec4(1.0, 1.0, 1.0, alpha);
}