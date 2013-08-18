uniform vec3 light;
uniform sampler2D texture;
uniform vec4 color;

varying vec2 v_texcoord;
varying vec3 v_normal;

void main()
{
    vec3 normal = normalize(v_normal);

    if (gl_FrontFacing == false)
        normal = -normal;

    float diffuse = max(dot(normal, normalize(light)), 0.0);

    gl_FragColor = color * texture2D(texture, v_texcoord) * diffuse;
}
