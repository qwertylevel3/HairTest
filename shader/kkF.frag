#version 400
//#ifdef GL_ES
//// Set default precision to medium
//precision mediump int;
//precision mediump float;
//#endif

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 vVaryingTexCoords;
in vec3 vVaryingView;
in vec3 vVaryingTangent;

uniform sampler2D texture;
uniform sampler2D hairTexture;

void main()
{
    //纹理
    vec4 tex=texture2D(texture,vVaryingTexCoords);
    gl_FragColor+=tex;

    //切线角度随机偏移
    vec4 tangentShift=texture2D(hairTexture,vVaryingTexCoords);
    tangentShift.r=(tangentShift.r-0.5)/15;
    vec3 vTangent=vVaryingTangent+tangentShift.r*vVaryingNormal;


    //漫反射强度
    float diff=max(0.0,dot(normalize(vVaryingNormal),normalize(vVaryingLightDir)));

    gl_FragColor+=diff*diffuseColor;

    //环境光
    gl_FragColor+=ambientColor;

//    //镜面光
//    vec3 vReflection= normalize(reflect(-normalize(vVaryingLightDir),normalize(vVaryingNormal)));
//    float spec = max(0.0,dot(normalize(vVaryingNormal),vReflection));

//    //镜面光
    vec3 vHalf= normalize(vVaryingLightDir+vVaryingView);
    float spec = max(0.0,pow(1-pow(dot(normalize(vTangent),vHalf),2.0),1.0/2.0));

    if(diff!=0)
    {
        float fSpec=pow(spec ,2048.0);
        gl_FragColor.rgb+=vec3(fSpec,fSpec,fSpec);
        gl_FragColor.a=tex.a;
    }
}

