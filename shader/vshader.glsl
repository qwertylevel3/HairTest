#version 400
//#ifdef GL_ES
//// Set default precision to medium
//precision mediump int;
//precision mediump float;
//#endif

in vec4 vVertex;
in vec3 vNormal;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;
uniform mat3 normalMatrix;
uniform vec3 vLightPosition;

out vec3 vVaryingNormal;
out vec3 vVaryingLightDir;

void main()
{
    vVaryingNormal=normalMatrix*vNormal;

    vec4 vPosition4=vMatrix*mMatrix*vVertex;
    vec3 vPosition3=vPosition4.xyz/vPosition4.w;

    vVaryingLightDir=normalize(vLightPosition-vPosition3);

    gl_Position=pMatrix*vMatrix*mMatrix*vVertex;
}
