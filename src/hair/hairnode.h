#ifndef HAIRNODE_H
#define HAIRNODE_H


#include"stable.h"

struct HairNode
{
    //前帧，本帧位置
    QVector3D p0,p1;

    //和上一节点止动长度
    float length;

    //质量
    float mass;
};

#endif // HAIRNODE_H
