
// OPENGL ES2 compliant shaders

// --- default.vs ---

uniform mat4 modelView;
uniform mat4 projection;

attribute vec4 position;
attribute vec4 color;

varying vec4 vColor;

void main()
{
    gl_Position = projection * modelView * position;
    vColor = color;
}



// --- default.fs ---

varying lowp vec4 vColor;

void main()
{
    gl_FragColor = vColor;
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
