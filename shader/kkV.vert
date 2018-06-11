#version 400
//#ifdef GL_ES
//// Set default precision to medium
//precision mediump int;
//precision mediump float;
//#endif

in vec4 vVertex;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 vTangent;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;
uniform mat3 normalMatrix;
uniform vec3 vLightPosition;
uniform vec3 cameraPos;

out vec3 vVaryingNormal;
out vec3 vVaryingLightDir;
out vec2 vVaryingTexCoords;
out vec3 vVaryingView;
out vec3 vVaryingTangent;

void main()
{
    //法线向量
    vVaryingNormal=normalMatrix*vNormal;

    vec4 vPosition4=mMatrix*vMatrix*vVertex;
    vec3 vPosition3=vPosition4.xyz/vPosition4.w;

    //到光线的单位向量
    vVaryingLightDir=normalize(vLightPosition-vPosition3);

    vVaryingTexCoords=vTexCoords;

    vec3 vVertex3=vVertex.xyz;

    vVaryingView=normalize(cameraPos-vVertex3);

    vVaryingTangent=vTangent;

    gl_Position=pMatrix*vMatrix*mMatrix*vVertex;
}
