#ifndef HAIRSTRAND_H
#define HAIRSTRAND_H

#include"stable.h"

struct HairStrand
{
    //此发束中节点数组起始和结束索引
    int nodeStart,nodeEnd;
    //发根坐标
    QVector3D rootPos;
};

#endif // HAIRSTRAND_H
