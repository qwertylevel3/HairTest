#include "hair.h"

Hair::Hair()
{

}

void Hair::update(Env &env, float dt)
{
    for(int i=0; i<strandBox.size(); i++)
    {
        HairStrand strand=strandBox[i];
        for(int index=strand.nodeStart+1; index<strand.nodeEnd; index++)
        {
            //计算加速度
            QVector3D a=calNodeForce(env,index)/nodeBox[index].mass;
            //计算verlet积分,求出新位置
            QVector3D p2=verlet(index,env.damping,dt,a);

            nodeBox[index].p0=nodeBox[index].p1;
            nodeBox[index].p1=p2;

            //update draw box
            drawNodeBox[index]=p2;
        }
    }

    for(int i=0; i<strandBox.size(); i++)
    {
        int iterationCount=3;

        while(iterationCount>0)
        {
            iterationCount--;

            HairStrand strand=strandBox[i];
            for(int index=strand.nodeStart+1; index<strand.nodeEnd; index++)
            {
                auto na=nodeBox[index-1];
                auto nb=nodeBox[index];

                //碰撞检测
                nb.p1=collideSphere(env.sphereBox,nb.p1);

                //长度约束
                nb.p1=lengthConstraint(na.p1,nb.p1,nb.length);
                nodeBox[index].p1=nb.p1;

            }
            //变换发根位置
//            auto root=nodeBox[strand.nodeStart];
//            nodeBox[strand.nodeStart].p1=transform(root.p1);
//            strand.rootPos=transform(strand.rootPos);
        }
    }

}

void Hair::init(QVector<QVector3D> &rootPosBox)
{
    mMatrix.setToIdentity();
    //生成头发节点
    for(int i=0; i<rootPosBox.size(); i++)
    {
        HairStrand strand;

        strand.nodeStart=nodeBox.size();

        //每根头发节点数量
        int nodeCount=20;
        auto tempPos=rootPosBox[i];
        while(nodeCount>0)
        {
            HairNode node;
            if(nodeCount==20)
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
    for(int i=0; i<strandBox.size(); i++)
    {
        int index=strandBox[i].nodeStart;
        while(index<strandBox[i].nodeEnd-1)
        {
            nodeIndex.append(index);
            nodeIndex.append(index+1);
            index++;
        }
    }
}

void Hair::setMMatrix(QMatrix4x4 m)
{
    mMatrix=m;
}

void Hair::rotate(float angle, float x, float y, float z)
{
    mMatrix.rotate(angle,x,y,z);
    for(int i=0; i<strandBox.size(); i++)
    {
        auto strand=strandBox[i];
        auto root=nodeBox[strand.nodeStart];
        nodeBox[strand.nodeStart].p1=transform(root.p1);
    }
}

QVector3D Hair::calNodeForce(Env &env, int nodeIndex)
{
    QVector3D vec;
    //全局风
    //xyz为方向，w为强度
    QVector4D wind=env.wind;
    wind.setX(wind.x()*wind.w());
    wind.setY(wind.y()*wind.w());
    wind.setZ(wind.z()*wind.w());

    vec.setX(wind.x());
    vec.setY(wind.y());
    vec.setZ(wind.z());

    //随机误差
    QVector3D randVec;
    randVec.setX(rand()%1);
    randVec.setY(rand()%1);
    randVec.setZ(rand()%1);

    vec+=randVec;

    float gravity=nodeBox[nodeIndex].mass*9.8;
    vec+= QVector3D(0,-gravity,0);
    return vec;
}

QVector3D Hair::verlet(int nodeIndex, float damping, float dt, QVector3D a)
{
    QVector3D p0=nodeBox[nodeIndex].p0;
    QVector3D p1=nodeBox[nodeIndex].p1;

    return p1+damping*(p1-p0)+a*dt*dt;
}

QVector3D Hair::collideSphere(QVector<Sphere*> &sphereBox, QVector3D p)
{
    for(int i=0; i<sphereBox.size(); i++)
    {
        float r=sphereBox[i]->getR();
        QVector3D center=sphereBox[i]->getCenter();

        if(center.distanceToPoint(p)<r)
        {
            QVector3D vec=p-center;
            vec.normalize();
            vec*=r;
            p=center+vec;
        }
    }
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
    //发根位置变换
    QVector4D temp=p.toVector4D();
    return (temp*mMatrix).toVector3D();
}

