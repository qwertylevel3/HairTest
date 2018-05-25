#ifndef SPHERE_H
#define SPHERE_H


//#include"stable.h"
#include"model.h"


//碰撞球
class Sphere:public Model
{
public:
    Sphere();
    bool init();
    float getR();
    QVector3D getCenter();
protected:
    float r;
    QVector3D pos;
};

#endif // SPHERE_H
