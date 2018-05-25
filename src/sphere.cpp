#include "sphere.h"

Sphere::Sphere()
{

}

bool Sphere::init()
{
    Model::init();

    QVector3D p1=points[0];
    QVector3D p2=points[0];

    float max=0;
    for(int i=0;i<points.size();i++)
    {
        auto curPos=points[i];
        float dis=p1.distanceToPoint(curPos);
        if(dis>max)
        {
            max=dis;
            p2=curPos;
        }
    }
    r=max/2;
    QVector3D vec=p2-p1;
    vec/=2;
    pos=p1+vec;

//    qDebug()<<r;
//    qDebug()<<pos.x()<<" "<<pos.y()<<" "<<pos.z();

    return true;
}

float Sphere::getR()
{
    return r;
}

QVector3D Sphere::getCenter()
{
    return pos;
}
