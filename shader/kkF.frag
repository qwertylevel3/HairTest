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
uniform sampler2D noiseTexture;

//主高光
float getMainSpec()
{
    //切线角度随机偏移
    vec4 tangentShift=texture2D(hairTexture,vVaryingTexCoords);
    tangentShift.r=(tangentShift.r-0.5)/50;
    vec3 vTangent=vVaryingTangent+tangentShift.r*vVaryingNormal;

    //镜面光
    vec3 vHalf= normalize(vVaryingLightDir+vVaryingView);
    float dotTH=dot(normalize(vTangent),vHalf);
    float sinTH=sqrt(1-dotTH*dotTH);
    float spec = max(0.0,sinTH);

    return pow(spec ,2048.0);
}

//噪点高光
float getNoiseSpec()
{
    //切线角度随机偏移
    vec4 tangentShift=texture2D(hairTexture,vVaryingTexCoords);
    tangentShift.r=(tangentShift.r-0.5)/2;
    vec3 vTangent=vVaryingTangent-tangentShift.r*vVaryingNormal;

    //镜面光
    vec3 vHalf= normalize(vVaryingLightDir+vVaryingView);
    float dotTH=dot(normalize(vTangent),vHalf);
    float sinTH=sqrt(1-dotTH*dotTH);
    float spec = max(0.0,sinTH);
    return pow(spec ,1024.0);
}

void main()
{
    //纹理
    vec4 tex=texture2D(texture,vVaryingTexCoords);
    gl_FragColor+=tex;

    //漫反射
    float diff=max(0.0,dot(normalize(vVaryingNormal),normalize(vVaryingLightDir)));
    gl_FragColor+=diff*diffuseColor;

    //环境光
    gl_FragColor+=ambientColor;

    if(diff!=0)
    {
        float mSpec=getMainSpec();
        float nSpec=getNoiseSpec();

        vec4 vSpec=specularColor*mSpec;

        vSpec+=specularColor*nSpec;


        gl_FragColor.rgb+=vSpec.rgb;
//        gl_FragColor.rgb+=vec3(nSpec,nSpec,nSpec);
//        gl_FragColor.rgb-=vec3(0.5,0.5,0.5);
        gl_FragColor.a=tex.a;
    }
}

