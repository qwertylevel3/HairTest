#include "line.h"

Line::Line()
{

}

bool Line::init()
{
    oriPoints.push_back(p1);
    oriPoints.push_back(p2);
    points.push_back(p1);
    points.push_back(p2);
    normals.push_back(p1);
    normals.push_back(p2);
    pointIndices.push_back(0);
    pointIndices.push_back(1);

    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(getIndicesData(), countIndices()* sizeof(GLuint));

    normalBuf.create();
    normalBuf.bind();
    normalBuf.allocate(getNormalsData(),countNormals()*sizeof(QVector3D));

    mMatrix.setToIdentity();
    return true;
}

void Line::setP1(QVector3D p)
{
    p1=p;
}

void Line::setP2(QVector3D p)
{
    p2=p;
}

void Line::update(Env &env, float dt)
{
    oriPoints[0]=p1;
    oriPoints[1]=p2;
    points[0]=p1;
    points[1]=p2;

    arrayBuf.bind();
    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));
}

void Line::draw(QOpenGLShaderProgram &shaderProgram)
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

    //    offset += sizeof(QVector3D);
    //
    //    int texcoordLocation = program.attributeLocation("a_texcoord");
    //    program.enableAttributeArray(texcoordLocation);
    //    program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    //glDrawElements(GL_POINTS, model->indexs(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_LINES, countIndices(), GL_UNSIGNED_INT, 0);
    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

}
