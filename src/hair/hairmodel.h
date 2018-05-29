#ifndef HAIRMODEL_H
#define HAIRMODEL_H


#include "model.h"
#include "hairnode.h"
#include"hairstrand.h"
#include"sphere.h"

class HairModel:public Model
{
public:
    HairModel();
    bool init();
    void draw(QOpenGLShaderProgram &shaderProgram);
    void update(Env& env,float dt);
    void rotate(float angle, float x, float y, float z);
protected:
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
        return &nodeIndexBox[0];
    }
    int countNodeIndex()
    {
        return nodeIndexBox.size();
    }
    void* getTexCoordData()
    {
        return &texCoordBox[0];
    }
    int countTexCoord()
    {
        return texCoordBox.size();
    }


    void updateBuf();

    //计算节点受力
    //1.重力
    //2.全局风
    QVector3D calNodeForce(Env& env,int nodeIndexBox);
    //verlet
    QVector3D verlet(int nodeIndexBox, float damping, float dt, QVector3D a);
    //碰撞检测
    QVector3D collideSphere(QVector<Sphere *> &sphereBox, QVector3D p);
    //长度约束
    QVector3D lengthConstraint(QVector3D p1,QVector3D p2,float length);

    //所有头发骨架节点
    QVector<HairNode> nodeBox;
    //所有发丝骨架
    QVector<HairStrand> strandBox;

    //绘制box,根据骨架展开为三角形
    //方便opengl直接绘制
    QVector<QVector3D> drawNodeBox;

    //纹理坐标位置
    QVector<QVector2D> texCoordBox;

    //头发节点索引
    //TODO:展开为三角形
    QVector<GLuint> nodeIndexBox;

    QOpenGLTexture* texture;

//    QOpenGLBuffer texBuf{QOpenGLBuffer::PixelPackBuffer};
    QOpenGLBuffer texBuf{QOpenGLBuffer::VertexBuffer};
//     VertexBuffer       = 0x8892, // GL_ARRAY_BUFFER
//    IndexBuffer         = 0x8893, // GL_ELEMENT_ARRAY_BUFFER
//    PixelPackBuffer     = 0x88EB, // GL_PIXEL_PACK_BUFFER
//    PixelUnpackBuffer   = 0x88EC  // GL_PIXEL_UNPACK_BUFFER

};

#endif // HAIRMODEL_H
