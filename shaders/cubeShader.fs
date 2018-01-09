#version 330 core

in vec3 TexCoord; // 代表3D纹理坐标的方向向量
out vec4 FragColor;

uniform samplerCube cubemap; // 立方体贴图的纹理采样器

void main()
{             
    FragColor = texture(cubemap, TexCoord);
}