//
//  volumetric_line.glsl
//  VolumetricLines
//
//  Created by Marcus Wendt on 23/05/2011.
//  Copyright 2011 Marcus Wendt. All rights reserved.
//


// --- test.vs ---
void main()
{
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}


// --- test.gs ---
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

// --- test.fs ---

varying vec4 vNormal;
varying float vPathCoord;

void main()
{
    gl_FragColor = gl_Color;
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//    gl_FragColor = vNormal + vec4(0.0, 0.0, 1.0, 1.0);
//    gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].xy);
}



// --- test2.gs ---
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


// --- VolumetricLine.vs ---

attribute vec4 Position;
attribute vec3 Normal;
attribute float PathCoord;

uniform mat4 ModelViewProjection;

varying vec3 vPosition;
varying vec3 vNormal;
varying float vPathCoord;

void main()
{
//    gl_Position = ModelViewProjection * Position;
    gl_Position = gl_ModelViewProjectionMatrix * Position;
    vPosition = Position.xyz;
    vNormal = Normal;
    vPathCoord = PathCoord;
}


// --- VolumetricLine.gs ---
#version 120
#extension GL_EXT_geometry_shader4 : enable

varying in vec3 vPosition[4];
varying in vec3 vNormal[4];
varying out vec2 gPosition;
varying out vec2 gEndpoints[2];

uniform float Radius;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 ModelViewProjection;

vec4 obb[8];

vec2 to_screen(vec3 v)
{
    v = (gl_ModelViewMatrix * vec4(v, 1)).xyz;
    vec4 u = gl_ProjectionMatrix * vec4(v, 1.0);
    return u.xy / u.w;
}

void emit()
{
    gPosition = gl_Position.xy / gl_Position.w;
    EmitVertex();
}

void emit(int a, int b, int c, int d)
{
    gl_Position = obb[a]; emit();
    gl_Position = obb[b]; emit();
    gl_Position = obb[c]; emit();
    gl_Position = obb[d]; emit();
    EndPrimitive();
}

void main()
{
    // Pass raytracing inputs to fragment shader:
    gEndpoints[0] = to_screen(vPosition[1]);
    gEndpoints[1] = to_screen(vPosition[2]);

    // Compute orientation vectors for the connecting faces:
    vec3 p0, p1, p2, p3;
    p0 = vPosition[0]; p1 = vPosition[1];
    p2 = vPosition[2]; p3 = vPosition[3];
    vec3 n0 = normalize(p1-p0);
    vec3 n1 = normalize(p2-p1);
    vec3 n2 = normalize(p3-p2);
    vec3 u = normalize(n0+n1);
    vec3 v = normalize(n1+n2);

    // Compute the eight corners:

    vec3 i,j,k; float r = Radius;

    j = u; i = vNormal[1]; k = cross(i, j);
    obb[0] = gl_ModelViewProjectionMatrix*vec4(p1 + i*r + k*r,1);
    obb[1] = gl_ModelViewProjectionMatrix*vec4(p1 + i*r - k*r,1);
    obb[2] = gl_ModelViewProjectionMatrix*vec4(p1 - i*r - k*r,1);
    obb[3] = gl_ModelViewProjectionMatrix*vec4(p1 - i*r + k*r,1);

    j = v; i = vNormal[2]; k = cross(i, j);
    obb[4] = gl_ModelViewProjectionMatrix*vec4(p2 + i*r + k*r,1);
    obb[5] = gl_ModelViewProjectionMatrix*vec4(p2 + i*r - k*r,1);
    obb[6] = gl_ModelViewProjectionMatrix*vec4(p2 - i*r - k*r,1);
    obb[7] = gl_ModelViewProjectionMatrix*vec4(p2 - i*r + k*r,1);

    // Emit the six faces of the prismoid:
    emit(0,1,3,2);
    emit(5,4,6,7);
    emit(4,5,0,1);
    emit(3,2,7,6);
    emit(0,3,4,7);
    emit(2,1,6,5);
}



// --- VolumetricLine.fs ---

uniform vec4 Color;

varying vec2 gEndpoints[2];
varying vec2 gPosition;

uniform float Radius;
uniform mat4 Projection;

float line_distance(vec2 pt, vec2 a, vec2 b)
{
    float dist = distance(a,b);
    vec2 v = normalize(b-a);
    float t = dot(v,pt-a);
    vec2 spinePoint;
    if (t > dist) spinePoint = b;
    else if (t > 0.0) spinePoint = a + t*v;
    else spinePoint = a;
    return distance(pt,spinePoint);
}

void main()
{
    vec2 x1 = gEndpoints[0];
    vec2 x2 = gEndpoints[1];
    float d = line_distance(gPosition,x1,x2);
    d = 1.0 - 12.0 * d;
    gl_FragColor = vec4(vec3(d), 1.0);
}

