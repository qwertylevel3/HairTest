#include "hairmodel.h"
#include "env.h"


HairModel::HairModel()
{

}

bool HairModel::init()
{
    texture=new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setFormat(QOpenGLTexture::RGBA8U);
    texture->setData(QImage("texture/hair.png").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    mMatrix.setToIdentity();
    //生成头发节点
    for(int i=0; i<oriPoints.size(); i++)
    {
        HairStrand strand;

        strand.nodeStart=nodeBox.size();

        //每根头发节点数量
        int nodeCount=10;
        auto tempPos=oriPoints[i];
        while(nodeCount>0)
        {
            HairNode node;
            if(nodeCount==10)
            {
                //首个节点距离上个节点止动长度为0
                node.length=0;
                tempPos.setY(tempPos.y());
            }
            else
            {
                node.length=0.2;
                tempPos.setY(tempPos.y()+0.2);
            }

            //单个节点质量
            //TODO:越靠近头皮质点质量越大
            node.mass=10;

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
            nodeIndexBox.append(index);
            nodeIndexBox.append(index+1);
            index++;
        }
        //图元重启标志点
        nodeIndexBox.append(0xffffffff);
    }

    for(int i=0; i<drawNodeBox.size(); i++)
    {
        texCoordBox.append(QVector2D(0,0));
    }

    texBuf.create();
    texBuf.bind();
    texBuf.allocate(getTexCoordData(),countTexCoord()*sizeof(QVector2D));

    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(getDrawNodeData(),countNode()*sizeof(QVector3D));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(getNodeIndexData(),countNodeIndex()*sizeof(GLuint));

    //TODO
    //头发每个点的法向量
    normalBuf.create();
    normalBuf.bind();
    normalBuf.allocate(getDrawNodeData(),countNode()*sizeof(QVector3D));

    return true;
}

void HairModel::draw(QOpenGLShaderProgram &shaderProgram)
{
    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);

    texture->bind();
    shaderProgram.setUniformValue("texture",0);

    normalBuf.bind();

    int normalLocation=shaderProgram.attributeLocation("vNormal");
    shaderProgram.enableAttributeArray(normalLocation);
    shaderProgram.setAttributeBuffer(normalLocation,GL_FLOAT,0,3,sizeof(QVector3D));

    arrayBuf.bind();
    indexBuf.bind();

    quintptr offset = 0;

    int vertexLocation = shaderProgram.attributeLocation("vVertex");
    shaderProgram.enableAttributeArray(vertexLocation);
    shaderProgram.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    //TODO
    //纹理
    // program.setUniformValue("vLightPosition",lightPos);

    texBuf.bind();
    int texcoordLocation = shaderProgram.attributeLocation("vTexCoords");
    shaderProgram.enableAttributeArray(texcoordLocation);
    shaderProgram.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(QVector2D));

//    glDrawElements(GL_LINE_STRIP, countNodeIndex(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLE_STRIP, countNodeIndex(), GL_UNSIGNED_INT, 0);

    glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
}

void HairModel::update(Env &env, float dt)
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

        }
    }

    //迭代碰撞检测，长度约束等
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
        }
    }

    //根据头发骨架展开三角形
    drawNodeBox.clear();
    nodeIndexBox.clear();
    texCoordBox.clear();
    for(int i=0; i<strandBox.size(); i++)
    {
        auto strand=strandBox[i];
        int count=0;
        for(int j=strand.nodeStart; j<strand.nodeEnd; j++)
        {
            //TODO:新加入节点与骨架节点构成三角形面向视口
            auto hairNode=nodeBox[j];
            QVector3D pos=hairNode.p1;

            drawNodeBox.append(pos);
            pos.setZ(pos.z()+0.1);
            drawNodeBox.append(pos);

            //展开的三角形索引表
            nodeIndexBox.append(j*2);
            nodeIndexBox.append(j*2+1);
            texCoordBox.append(QVector2D(float(count)*0.1,0));
            texCoordBox.append(QVector2D(float(count)*0.1,1.0));
            count++;
        }
        //图元重启标志点
        nodeIndexBox.append(0xffffffff);
    }

    //更新opengl缓存
    updateBuf();
}

void HairModel::rotate(float angle, float x, float y, float z)
{
//    mMatrix.rotate(angle,x,y,z);
//    hair.setMMatrix(mMatrix);
//    mMatrix.rotate(angle,x,y,z);
    QMatrix4x4 m;
    m.rotate(angle,x,y,z);
    for(int i=0; i<strandBox.size(); i++)
    {
        auto strand=strandBox[i];
        auto root=nodeBox[strand.nodeStart].p1;
        QVector4D temp=root.toVector4D();
        auto newPos=(temp*m).toVector3D();

        nodeBox[strand.nodeStart].p1=newPos;
        drawNodeBox[strand.nodeStart]=newPos;
    }
}


void HairModel::updateBuf()
{
    arrayBuf.bind();
    arrayBuf.allocate(getDrawNodeData(),countNode()*sizeof(QVector3D));

    indexBuf.bind();
    indexBuf.allocate(getNodeIndexData(),countNodeIndex()*sizeof(GLuint));

    normalBuf.bind();
    normalBuf.allocate(getDrawNodeData(),countNode()*sizeof(QVector3D));

    texBuf.bind();
    texBuf.allocate(getTexCoordData(),countTexCoord()*sizeof(QVector2D));
}

QVector3D HairModel::calNodeForce(Env &env, int nodeIndex)
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

QVector3D HairModel::verlet(int nodeIndex, float damping, float dt, QVector3D a)
{
    QVector3D p0=nodeBox[nodeIndex].p0;
    QVector3D p1=nodeBox[nodeIndex].p1;

    return p1+damping*(p1-p0)+a*dt*dt;
}

QVector3D HairModel::collideSphere(QVector<Sphere *> &sphereBox, QVector3D p)
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

QVector3D HairModel::lengthConstraint(QVector3D p1, QVector3D p2, float length)
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


