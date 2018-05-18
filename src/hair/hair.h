#ifndef HAIR_H
#define HAIR_H

#include"stable.h"

#include"hairnode.h"
#include"hairstrand.h"
#include"sphere.h"


class Hair
{
public:
    Hair();
    void update(QVector<Sphere>& sphereBox,float damping,float dt);
protected:
    QVector3D calNodeForce(int nodeIndex);
    //verlet
    QVector3D verlet(int nodeIndex, float damping, float dt, QVector3D a);

    //碰撞检测
    QVector3D collideSphere(QVector<Sphere>& sphereBox,QVector3D p);

    //长度约束
    QVector3D lengthConstraint(QVector3D p1,QVector3D p2,float length);

    //发根位置变换
    QVector3D transform(QVector3D p);

    //所有头发节点
    QVector<HairNode> nodeBox;
    //所有发丝
    QVector<HairStrand> strandBox;

    QVector3D pos;
    QMatrix4x4 mMatrix;

};

#endif // HAIR_H
