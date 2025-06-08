#type vertex
#version 460 core

layout(location=0) in vec3  a_Position;
layout(location=1) in vec4  a_Color;
layout(location=2) in vec2  a_TexCoords;
layout(location=3) in float a_TextureId;

layout(location=4) in mat4  a_Transform;
layout(location=8) in float a_InstanceTexId;

uniform mat4 u_ProjectionView;

out vec4  v_Color;
out vec2  v_TexCoords;
out float v_TextureId;

void main()
{
	v_Color     = a_Color;
	v_TexCoords = a_TexCoords;
	v_TextureId  = a_InstanceTexId;
	gl_Position = u_ProjectionView * a_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

uniform sampler2D u_Textures[32];

in vec4  v_Color;
in vec2  v_TexCoords;
in float v_TextureId;

layout(location=0) out vec4 a_FragColor;

void main()
{
	vec4 color = vec4(1, 1, 1, 1);

    switch(int(v_TextureId))
    {
        case  0: color *= texture(u_Textures[ 0], v_TexCoords) * v_Color; break;
        case  1: color *= texture(u_Textures[ 1], v_TexCoords) * v_Color; break;
        case  2: color *= texture(u_Textures[ 2], v_TexCoords) * v_Color; break;
        case  3: color *= texture(u_Textures[ 3], v_TexCoords) * v_Color; break;
        case  4: color *= texture(u_Textures[ 4], v_TexCoords) * v_Color; break;
        case  5: color *= texture(u_Textures[ 5], v_TexCoords) * v_Color; break;
        case  6: color *= texture(u_Textures[ 6], v_TexCoords) * v_Color; break;
        case  7: color *= texture(u_Textures[ 7], v_TexCoords) * v_Color; break;
        case  8: color *= texture(u_Textures[ 8], v_TexCoords) * v_Color; break;
        case  9: color *= texture(u_Textures[ 9], v_TexCoords) * v_Color; break;
        case 10: color *= texture(u_Textures[10], v_TexCoords) * v_Color; break;
        case 11: color *= texture(u_Textures[11], v_TexCoords) * v_Color; break;
        case 12: color *= texture(u_Textures[12], v_TexCoords) * v_Color; break;
        case 13: color *= texture(u_Textures[13], v_TexCoords) * v_Color; break;
        case 14: color *= texture(u_Textures[14], v_TexCoords) * v_Color; break;
        case 15: color *= texture(u_Textures[15], v_TexCoords) * v_Color; break;
        case 16: color *= texture(u_Textures[16], v_TexCoords) * v_Color; break;
        case 17: color *= texture(u_Textures[17], v_TexCoords) * v_Color; break;
        case 18: color *= texture(u_Textures[18], v_TexCoords) * v_Color; break;
        case 19: color *= texture(u_Textures[19], v_TexCoords) * v_Color; break;
        case 20: color *= texture(u_Textures[20], v_TexCoords) * v_Color; break;
        case 21: color *= texture(u_Textures[21], v_TexCoords) * v_Color; break;
        case 22: color *= texture(u_Textures[22], v_TexCoords) * v_Color; break;
        case 23: color *= texture(u_Textures[23], v_TexCoords) * v_Color; break;
        case 24: color *= texture(u_Textures[24], v_TexCoords) * v_Color; break;
        case 25: color *= texture(u_Textures[25], v_TexCoords) * v_Color; break;
        case 26: color *= texture(u_Textures[26], v_TexCoords) * v_Color; break;
        case 27: color *= texture(u_Textures[27], v_TexCoords) * v_Color; break;
        case 28: color *= texture(u_Textures[28], v_TexCoords) * v_Color; break;
        case 29: color *= texture(u_Textures[29], v_TexCoords) * v_Color; break;
        case 30: color *= texture(u_Textures[30], v_TexCoords) * v_Color; break;
        case 31: color *= texture(u_Textures[31], v_TexCoords) * v_Color; break;
    }

	a_FragColor = color;
}