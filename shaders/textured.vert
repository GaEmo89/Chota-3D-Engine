#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_UV;

void main()
{
    vec4 world = u_Model * vec4(aPos, 1.0);
    v_FragPos = world.xyz;
    v_Normal  = mat3(transpose(inverse(u_Model))) * aNormal;
    v_UV      = aUV;

    gl_Position = u_Projection * u_View * world;
}
