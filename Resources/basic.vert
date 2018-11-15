#version 400

uniform mat4 ModelViewProjectionMatrix;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec4 VertexColor;


varying vec4 Fcolor;
varying vec3 Fnorm;
void main()
{
Fcolor = VertexColor;
Fnorm = VertexNormal;
gl_Position = ModelViewProjectionMatrix * vec4(VertexPosition.xyz, 1.0f);
}



/*------------------------------------------------------------------------------------- 
PerVertex Vertex Lighting shader


#version 120

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec4 VertexColor;

varying vec4 color;
void main()
{
vec3 lpos = vec3(0,0,0);
vec3 nVertexNormal = normalize(VertexNormal);
 vec3 modelViewVertex = vec3(gl_ModelViewMatrix * vec4(VertexPosition, 1.0));
 vec3 modelViewNormal = vec3(gl_ModelViewMatrix * vec4(nVertexNormal, 0.0));
 float distance = length(lpos - modelViewVertex);
 vec3 lightVector = normalize(lpos - modelViewVertex);
 float diffuse = max(dot(modelViewNormal, lightVector), 1.0);
 diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));


color = VertexColor * diffuse;
gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition.xyz, 1.0f);
}
________________________________________________________________________________________*/










/*  PerPixel Vertex shader
_________________________________________________________________________________________________________

uniform mat4 u_MVPMatrix;      // A constant representing the combined model/view/projection matrix.
uniform mat4 u_MVMatrix;       // A constant representing the combined model/view matrix.
 
attribute vec4 a_Position;     // Per-vertex position information we will pass in.
attribute vec4 a_Color;        // Per-vertex color information we will pass in.
attribute vec3 a_Normal;       // Per-vertex normal information we will pass in.
 
varying vec3 v_Position;       // This will be passed into the fragment shader.
varying vec4 v_Color;          // This will be passed into the fragment shader.
varying vec3 v_Normal;         // This will be passed into the fragment shader.
 
// The entry point for our vertex shader.
void main()
{
    // Transform the vertex into eye space.
    v_Position = vec3(u_MVMatrix * a_Position);
 
    // Pass through the color.
    v_Color = a_Color;
 
    // Transform the normal's orientation into eye space.
    v_Normal = vec3(u_MVMatrix * vec4(a_Normal, 0.0));
 
    // gl_Position is a special variable used to store the final position.
    // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
    gl_Position = u_MVPMatrix * a_Position;
}
_______________________________________________________________________________________________________*/