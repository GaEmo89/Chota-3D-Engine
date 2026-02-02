#version 460 core
out vec4 FragColor;

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_UV;

uniform sampler2D u_Texture;
uniform vec3 u_LightDir;
uniform vec3 u_ViewPos;

void main()
{
    vec3 base = texture(u_Texture, v_UV).rgb;

    vec3 n = normalize(v_Normal);
    vec3 l = normalize(-u_LightDir);

    float ambient = 0.20;
    float diff = max(dot(n, l), 0.0);

    vec3 v = normalize(u_ViewPos - v_FragPos);
    vec3 r = reflect(-l, n);
    float spec = pow(max(dot(v, r), 0.0), 32.0);

    vec3 color = (ambient + diff) * base + spec * vec3(1.0);
    FragColor = vec4(color, 1.0);
}
