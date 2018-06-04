#include "bcurve.h"

BCurve::BCurve()
{

}

bool BCurve::init()
{
    //填充iterCount个点
    for(int i=0; i<iterCount; i++)
    {
        points.append(QVector3D(0,0,0));
        //TODO
        //每个点的向量方向
        normals.append(QVector3D(0,0,0));

    }
    for(int i=0; i<iterCount-1; i++)
    {
        pointIndices.append(i);
        pointIndices.append(i+1);
    }

    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(getIndicesData(), countIndices()* sizeof(GLuint));

    normalBuf.create();
    normalBuf.bind();
    normalBuf.allocate(getNormalsData(),countNormals()*sizeof(QVector3D));

    return true;
}

void BCurve::draw(QOpenGLShaderProgram &shaderProgram)
{
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


    //glDrawElements(GL_POINTS, model->indexs(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_LINES, countIndices(), GL_UNSIGNED_INT, 0);
    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

}

void BCurve::update(Env &env, float dt)
{
    Q_UNUSED(env);
    Q_UNUSED(dt);

    points.clear();
    for(int i=0; i<iterCount; i++)
    {
        points.append(fun(float(i)*(1.0/float(iterCount))));
    }

    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));
}

void BCurve::setP1(QVector3D p)
{
    p1=p;
}

void BCurve::setP2(QVector3D p)
{
    p2=p;
}

void BCurve::setP3(QVector3D p)
{
    p3=p;
}

void BCurve::setP4(QVector3D p)
{
    p4=p;
}

QVector<QVector3D> &BCurve::getCurPointsRef()
{
    return points;
}

QVector3D BCurve::fun(float t)
{
    QVector4D temp;
    temp.setX(-(t*t*t)/6.0+(t*t)/2.0-t/2.0+1.0/6.0);
    temp.setY((t*t*t)/2.0-(t*t)+2.0/3.0);
    temp.setZ(-(t*t*t)/2.0+(t*t)/2.0+t/2.0+1.0/6.0);
    temp.setW(t*t*t/6.0);

    QVector3D ret;
    ret.setX(temp.x()*p1.x()+temp.y()*p2.x()+temp.z()*p3.x()+temp.w()*p4.x());
    ret.setY(temp.x()*p1.y()+temp.y()*p2.y()+temp.z()*p3.y()+temp.w()*p4.y());
    ret.setZ(temp.x()*p1.z()+temp.y()*p2.z()+temp.z()*p3.z()+temp.w()*p4.z());

    return ret;
}
