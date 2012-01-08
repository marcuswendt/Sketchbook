//
//  geoshader_test.glsl.h
//  VolumetricLines
//
//  Created by Marcus Wendt on 24/05/2011.
//  Copyright 2011 Marcus Wendt. All rights reserved.
//


// --- VS ---
void main()
{
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}


// --- GS ---
#version 120 
#extension GL_EXT_geometry_shader4 : enable

void main(void)
{   
    int i;

    for(i=0; i< gl_VerticesIn; i++){
        gl_Position = gl_PositionIn[i];
        gl_FrontColor = vec4(1.0, 0.0, 0.0, 1.0);
        EmitVertex();
    }
    EndPrimitive();					

    for(i=0; i< gl_VerticesIn; i++){
        gl_Position = gl_PositionIn[i];
        gl_Position.xy = gl_Position.yx;
        gl_FrontColor = vec4(0.0, 0.0, 1.0, 1.0);
        EmitVertex();
    }
    EndPrimitive();	
}


// --- FS ---
void main()
{
    gl_FragColor = gl_Color;
}
