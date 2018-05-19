#ifndef ENV_H
#define ENV_H

#include"stable.h"
#include"sphere.h"

struct Env
{
    QVector<Sphere> sphereBox;
    float damping{0.1};
    QVector4D wind;
};

#endif // ENV_H
