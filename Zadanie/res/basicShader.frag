#version 330

out vec4 out_color;

in vec2 texCoord;

//uniform vec4 u_Color;

uniform sampler2D tex;

void main()
{
	//out_color = u_Color;
	//out_color = vec4(1.0, 0.0, 0.0, 1.0);
	out_color = texture2D(tex, texCoord);
}