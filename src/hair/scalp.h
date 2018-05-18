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
protected:
    Hair hair;
};

#endif // SCALP_H
