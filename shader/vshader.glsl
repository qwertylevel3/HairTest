#version 400
//#ifdef GL_ES
//// Set default precision to medium
//precision mediump int;
//precision mediump float;
//#endif

in vec4 vVertex;
in vec3 vNormal;
in vec2 vTexCoords;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;
uniform mat3 normalMatrix;
uniform vec3 vLightPosition;

out vec3 vVaryingNormal;
out vec3 vVaryingLightDir;
out vec2 vVaryingTexCoords;

void main()
{
    vVaryingNormal=normalMatrix*vNormal;

    vec4 vPosition4=mMatrix*vMatrix*vVertex;
    vec3 vPosition3=vPosition4.xyz/vPosition4.w;

    vVaryingLightDir=normalize(vLightPosition-vPosition3);
    vVaryingTexCoords=vTexCoords;

    gl_Position=pMatrix*vMatrix*mMatrix*vVertex;
}
