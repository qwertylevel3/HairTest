#ifndef HAIR_H
#define HAIR_H

#include"stable.h"

#include"hairnode.h"
#include"hairstrand.h"
#include"sphere.h"
#include"env.h"


class Hair
{
public:
    Hair();
    //TODO
    //节点更新放入vshader，GPU并行
    virtual void update(Env& env,float dt);

    //根据发根位置初始化发丝
    void init(QVector<QVector3D>& rootPosBox);

    void* getDrawNodeData()
    {
        return &drawNodeBox[0];
    }
    int countNode()
    {
        return nodeBox.size();
    }

    void *getNodeIndexData()
    {
        return &nodeIndex[0];
    }
    int countIndex()
    {
        return nodeIndex.size();
    }
protected:
    QVector3D calNodeForce(Env& env,int nodeIndex);
    //verlet
    QVector3D verlet(int nodeIndex, float damping, float dt, QVector3D a);

    //碰撞检测
    QVector3D collideSphere(QVector<Sphere>& sphereBox,QVector3D p);

    //长度约束
    QVector3D lengthConstraint(QVector3D p1,QVector3D p2,float length);

    //发根位置变换
    QVector3D transform(QVector3D p);

    //绘制box,需要和头发节点同步
    //方便opengl直接绘制
    QVector<QVector3D> drawNodeBox;

    //所有头发节点
    QVector<HairNode> nodeBox;
    //所有发丝
    QVector<HairStrand> strandBox;

    //头发节点索引
    //TODO:展开为三角形
    QVector<GLuint> nodeIndex;

    QVector3D pos;
    QMatrix4x4 mMatrix;
};

#endif // HAIR_H
