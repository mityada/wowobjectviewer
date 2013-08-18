uniform vec3 light;
uniform sampler2D texture;
uniform float alpha;

varying vec2 v_texcoord;
varying vec3 v_normal;

void main()
{
    float diffuse = max(dot(normalize(v_normal), normalize(light)), 0.0);

    gl_FragColor = texture2D(texture, v_texcoord) * diffuse;
    gl_FragColor.a = min(gl_FragColor.a, alpha); 
}
