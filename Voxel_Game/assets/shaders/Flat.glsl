#type vertex
#version 460 core

layout(location=0) in vec3 a_Position;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location=0) out vec4 a_FragColor;

uniform vec4 u_Color;

void main()
{
	a_FragColor = u_Color;
}