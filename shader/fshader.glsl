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

uniform sampler2D texture;

void main()
{
    //纹理
    gl_FragColor+=texture2D(texture,vVaryingTexCoords);

    //漫反射强度
    float diff=max(0.0,dot(normalize(vVaryingNormal),normalize(vVaryingLightDir)));

    gl_FragColor+=diff*diffuseColor;

    //环境光
    gl_FragColor+=ambientColor;

    //镜面光
    vec3 vReflection= normalize(reflect(-normalize(vVaryingLightDir),
                                        normalize(vVaryingNormal)));

    float spec = max(0.0,dot(normalize(vVaryingNormal),vReflection));

    if(diff!=0)
    {
        float fSpec=pow(spec ,128.0);
        gl_FragColor.rgb+=vec3(fSpec,fSpec,fSpec);
    }
}

