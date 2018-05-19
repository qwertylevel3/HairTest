#ifndef SCALP_H
#define SCALP_H


#include "model.h"
#include"hair.h"

class Scalp:public Model
{
public:
    Scalp();
    bool init();
    void draw(QOpenGLShaderProgram &shaderProgram);
    virtual void update(Env& env,float dt);
protected:
    void updateBuf();
    Hair hair;

};

#endif // SCALP_H
