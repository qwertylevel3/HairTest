#include "hair.h"

Hair::Hair()
{

}

void Hair::update(QVector<Sphere> &sphereBox, float damping, float dt)
{
    for(int i=0;i<nodeBox.size();i++)
    {
        QVector3D a=calNodeForce(i)/nodeBox[i].mass;
        QVector3D p2=verlet(i,damping,dt,a);

        nodeBox[i].p0=nodeBox[i].p1;
        nodeBox[i].p1=p2;
    }

    int iterationCount=10;
    for(int i=0;i<strandBox.size();i++)
    {
        iterationCount--;
        while(iterationCount>0)
        {
            HairStrand strand=strandBox[i];
            for(int index=strand.nodeStart;index<strand.nodeEnd-1;index++)
            {
                auto na=nodeBox[index];
                auto nb=nodeBox[index+1];

                //碰撞检测
                nb.p1=collideSphere(sphereBox,nb.p1);

                //长度约束
                na.p1=lengthConstraint(na.p1,nb.p1,nb.length);

            }
            //变换发根位置
            strand.rootPos=transform(strand.rootPos);
        }
    }

}

QVector3D Hair::calNodeForce(int nodeIndex)
{
    //求出每个节点的合力
    //暂时仅仅是返回重力
    float gravity=nodeBox[nodeIndex].mass*9.8;
    return QVector3D(0,0,gravity);
}

QVector3D Hair::verlet(int nodeIndex, float damping, float dt, QVector3D a)
{
    QVector3D p0=nodeBox[nodeIndex].p0;
    QVector3D p1=nodeBox[nodeIndex].p1;

    return p1+damping*(p1-p0)+a*dt*dt;
}

QVector3D Hair::collideSphere(QVector<Sphere> &sphereBox, QVector3D p)
{
    //TODO:
    //将p推到球面
    return p;
}

QVector3D Hair::lengthConstraint(QVector3D p1, QVector3D p2, float length)
{
    //TODO:
    //长度约束
    return p2;
}

QVector3D Hair::transform(QVector3D p)
{
    //TODO
    //发根位置变换
    return p;
}

