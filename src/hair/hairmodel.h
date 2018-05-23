#ifndef HAIRMODEL_H
#define HAIRMODEL_H


#include "model.h"
#include"hair.h"

class HairModel:public Model
{
public:
    HairModel();
    bool init();
    void draw(QOpenGLShaderProgram &shaderProgram);
    void update(Env& env,float dt);
    void rotate(float angle, float x, float y, float z);
protected:
    void updateBuf();
    Hair hair;

};

#endif // HAIRMODEL_H
