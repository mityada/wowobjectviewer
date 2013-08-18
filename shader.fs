uniform sampler2D texture;
uniform float alpha;

varying vec2 v_texcoord;

void main()
{
    gl_FragColor = texture2D(texture, v_texcoord);
    gl_FragColor.a = min(gl_FragColor.a, alpha); 
}
