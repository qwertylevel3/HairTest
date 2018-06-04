#ifndef BCURVE_H
#define BCURVE_H


#include "model.h"

class BCurve:public Model
{
public:
    BCurve();
    bool init();
    void draw(QOpenGLShaderProgram &shaderProgram);
    void update(Env &env, float dt);
    void setP1(QVector3D p);
    void setP2(QVector3D p);
    void setP3(QVector3D p);
    void setP4(QVector3D p);

    QVector<QVector3D> &getCurPointsRef();
protected:
    //函数曲线
    QVector3D fun(float t);

    QVector3D p1;
    QVector3D p2;
    QVector3D p3;
    QVector3D p4;
    int iterCount{50};
    QMatrix4x4 bMatrix;
};

#endif // BCURVE_H
