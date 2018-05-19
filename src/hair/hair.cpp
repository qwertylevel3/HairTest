#include "hair.h"

Hair::Hair()
{

}

void Hair::update(QVector<Sphere> &sphereBox, float damping, float dt)
{
    for(int i=0; i<strandBox.size(); i++)
    {
        HairStrand strand=strandBox[i];
        for(int index=strand.nodeStart+1; index<strand.nodeEnd; index++)
        {
            //计算加速度
            QVector3D a=calNodeForce(index)/nodeBox[index].mass;
            //计算verlet积分,求出新位置
            QVector3D p2=verlet(index,damping,dt,a);

            nodeBox[index].p0=nodeBox[index].p1;
            nodeBox[index].p1=p2;

            //update draw box
            drawNodeBox[index]=p2;
        }
    }

    for(int i=0; i<strandBox.size(); i++)
    {
        int iterationCount=2;

        while(iterationCount>0)
        {
            iterationCount--;

            HairStrand strand=strandBox[i];
            for(int index=strand.nodeStart+1; index<strand.nodeEnd; index++)
            {
                auto na=nodeBox[index-1];
                auto nb=nodeBox[index];

                //碰撞检测
                nb.p1=collideSphere(sphereBox,nb.p1);

                //长度约束
                nb.p1=lengthConstraint(na.p1,nb.p1,nb.length);
                nodeBox[index].p1=nb.p1;

            }
            //变换发根位置
            strand.rootPos=transform(strand.rootPos);
        }
    }
}

void Hair::init(QVector<QVector3D> &rootPosBox)
{
    //生成头发节点
    for(int i=0; i<rootPosBox.size(); i++)
    {
        HairStrand strand;

        strand.rootPos=rootPosBox[i];
        strand.nodeStart=nodeBox.size();

        int nodeCount=10;
        auto tempPos=rootPosBox[i];
        while(nodeCount>0)
        {
            HairNode node;
            if(nodeCount==10)
            {
                //首个节点距离上个节点止动长度为0
                node.length=0;
            }
            else
            {
                node.length=0.1;
            }

            node.mass=10;

            tempPos.setY(tempPos.y()+0.1);
            node.p0=tempPos;
            node.p1=tempPos;
            nodeBox.push_back(node);
            drawNodeBox.push_back(tempPos);

            nodeCount--;
        }

        strand.nodeEnd=nodeBox.size();
        strandBox.push_back(strand);
    }

    //生成头发节点索引
    for(int i=0; i<nodeBox.size(); i++)
    {
        nodeIndex.append(i);
    }
}

QVector3D Hair::calNodeForce(int nodeIndex)
{
    //求出每个节点的合力
    //暂时仅仅是返回重力
    float gravity=nodeBox[nodeIndex].mass*9.8;
    return QVector3D(0,-gravity,0);
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
    float tempLength= p1.distanceToPoint(p2);
    if(tempLength<length)
    {
        return p2;
    }
    else
    {
        QVector3D vec=p2-p1;
        //float dis=vec.length();
        vec.normalize();

        vec*=length;

        p1+=vec;

        return p1;
    }
}

QVector3D Hair::transform(QVector3D p)
{
    //TODO
    //发根位置变换
    return p;
}

