#ifndef LINE_H
#define LINE_H


#include"model.h"

//线
class Line:public Model
{
public:
    Line();
    bool init();
    void setP1(QVector3D p);
    void setP2(QVector3D p);
    void update(Env &env, float dt);
    void draw(QOpenGLShaderProgram &shaderProgram);
protected:
    QVector3D p1;
    QVector3D p2;
};

#endif // LINE_H
