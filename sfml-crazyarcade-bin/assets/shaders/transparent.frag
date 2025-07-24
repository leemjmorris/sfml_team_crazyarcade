uniform sampler2D texture;
uniform vec4 maskColor;
uniform float threshold;

void main()
{
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
    float diff = distance(texColor.rgb, maskColor.rgb);
    if (diff < threshold)
        discard;
    gl_FragColor = texColor;
}