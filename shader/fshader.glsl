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

void main()
{
    float diff=max(0.0,dot(normalize(vVaryingNormal),normalize(vVaryingLightDir)));

    gl_FragColor=diff*diffuseColor;

    gl_FragColor+=ambientColor;

    vec3 vReflection= normalize(reflect(-normalize(vVaryingLightDir),normalize(vVaryingNormal)));

    float spec = max(0.0,dot(normalize(vVaryingNormal),vReflection));

    if(diff!=0)
    {
        float fSpec=pow(spec ,128.0);
        gl_FragColor.rgb+=vec3(fSpec,fSpec,fSpec);
    }
}

