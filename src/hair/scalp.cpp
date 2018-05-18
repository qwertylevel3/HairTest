#include "scalp.h"


Scalp::Scalp()
{

}

bool Scalp::init()
{
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

void Scalp::draw(QOpenGLShaderProgram &shaderProgram)
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
    glDrawElements(GL_TRIANGLES, countIndices(), GL_UNSIGNED_INT, 0);
    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

    //    glDrawElements(GL_LINES, model->m_edgeIndices.size(), GL_UNSIGNED_INT, model->m_edgeIndices.data());

}
