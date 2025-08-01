uniform sampler2D texture;
uniform vec2 texelSize;
uniform float thickness;
uniform vec4 outlineColor;

void main() {
    float a = texture2D(texture, gl_TexCoord[0].xy).a;

    float neighborAlpha = 0.0;
    for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y)
            neighborAlpha = max(neighborAlpha,
                texture2D(texture,
                          gl_TexCoord[0].xy +
                          vec2(x, y) * texelSize * thickness).a);

    if (a == 0.0 && neighborAlpha > 0.0)
        gl_FragColor = outlineColor;
    else
        gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
