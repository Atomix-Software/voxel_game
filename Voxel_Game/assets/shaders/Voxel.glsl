#type vertex
#version 460 core

layout(location=0) in vec3  a_Position;
layout(location=1) in vec4  a_Color;
layout(location=2) in vec2  a_TexCoords;
layout(location=3) in float a_TextureId;
layout(location=4) in float a_FaceIndex;

struct InstanceData {
    mat4  Transform;
    int BlockId;
    int _pad0;
    int _pad1;
    int _pad2;
};

layout(std430, binding=0) buffer InstanceBuffer {
    InstanceData instances[];
};

uniform mat4 u_ProjectionView;

out vec4  v_Color;
out vec2  v_TexCoords;

flat out float v_TextureId;
flat out int   v_FaceIndex;
flat out int   v_BlockId;

void main()
{
    InstanceData data = instances[gl_InstanceID];

	v_Color     = a_Color;
	v_TexCoords = a_TexCoords;
	v_TextureId = a_TextureId;

    v_FaceIndex = int(a_FaceIndex);
    v_BlockId   = data.BlockId;

	gl_Position  = u_ProjectionView * data.Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

const float TILE_SIZE = 1.0 / 16.0;

struct FaceUV {
    vec2 Offset;
    vec2 Scale;
};

struct BlockUVData {
    FaceUV Faces[6]; // 6 faces per block
};

uniform sampler2D u_Textures[32];

in vec4  v_Color;
in vec2  v_TexCoords;

flat in float v_TextureId;
flat in int   v_FaceIndex;
flat in int   v_BlockId;

layout(location=0) out vec4 a_FragColor;

layout(std430, binding=1) buffer UVBuffer {
    BlockUVData BlockUVs[];
};

vec2 GetBlockUV(int blockId, int faceIndex, vec2 baseUV) {
    FaceUV face = BlockUVs[blockId].Faces[faceIndex];
    return baseUV * face.Scale + face.Offset;
}

void main()
{
	vec4 color = vec4(1, 1, 1, 1);
    vec2 blockUV = GetBlockUV(v_BlockId, v_FaceIndex, v_TexCoords);

    switch(int(v_TextureId))
    {
        case  0: color *= texture(u_Textures[ 0], blockUV) * v_Color; break;
        case  1: color *= texture(u_Textures[ 1], blockUV) * v_Color; break;
        case  2: color *= texture(u_Textures[ 2], blockUV) * v_Color; break;
        case  3: color *= texture(u_Textures[ 3], blockUV) * v_Color; break;
        case  4: color *= texture(u_Textures[ 4], blockUV) * v_Color; break;
        case  5: color *= texture(u_Textures[ 5], blockUV) * v_Color; break;
        case  6: color *= texture(u_Textures[ 6], blockUV) * v_Color; break;
        case  7: color *= texture(u_Textures[ 7], blockUV) * v_Color; break;
        case  8: color *= texture(u_Textures[ 8], blockUV) * v_Color; break;
        case  9: color *= texture(u_Textures[ 9], blockUV) * v_Color; break;
        case 10: color *= texture(u_Textures[10], blockUV) * v_Color; break;
        case 11: color *= texture(u_Textures[11], blockUV) * v_Color; break;
        case 12: color *= texture(u_Textures[12], blockUV) * v_Color; break;
        case 13: color *= texture(u_Textures[13], blockUV) * v_Color; break;
        case 14: color *= texture(u_Textures[14], blockUV) * v_Color; break;
        case 15: color *= texture(u_Textures[15], blockUV) * v_Color; break;
        case 16: color *= texture(u_Textures[16], blockUV) * v_Color; break;
        case 17: color *= texture(u_Textures[17], blockUV) * v_Color; break;
        case 18: color *= texture(u_Textures[18], blockUV) * v_Color; break;
        case 19: color *= texture(u_Textures[19], blockUV) * v_Color; break;
        case 20: color *= texture(u_Textures[20], blockUV) * v_Color; break;
        case 21: color *= texture(u_Textures[21], blockUV) * v_Color; break;
        case 22: color *= texture(u_Textures[22], blockUV) * v_Color; break;
        case 23: color *= texture(u_Textures[23], blockUV) * v_Color; break;
        case 24: color *= texture(u_Textures[24], blockUV) * v_Color; break;
        case 25: color *= texture(u_Textures[25], blockUV) * v_Color; break;
        case 26: color *= texture(u_Textures[26], blockUV) * v_Color; break;
        case 27: color *= texture(u_Textures[27], blockUV) * v_Color; break;
        case 28: color *= texture(u_Textures[28], blockUV) * v_Color; break;
        case 29: color *= texture(u_Textures[29], blockUV) * v_Color; break;
        case 30: color *= texture(u_Textures[30], blockUV) * v_Color; break;
        case 31: color *= texture(u_Textures[31], blockUV) * v_Color; break;
    }

	a_FragColor = color;
}