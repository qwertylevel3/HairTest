#include "hairmodel.h"


HairModel::HairModel()
{

}

bool HairModel::init()
{
//    arrayBuf.create();
//    arrayBuf.bind();
//    arrayBuf.allocate(getPointsData(), countPoints() * sizeof(QVector3D));
//
//    indexBuf.create();
//    indexBuf.bind();
//    indexBuf.allocate(getIndicesData(), countIndices()* sizeof(GLuint));
//
//    normalBuf.create();
//    normalBuf.bind();
//    normalBuf.allocate(getNormalsData(),countNormals()*sizeof(QVector3D));

    hair.init(oriPoints);

    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(hair.getDrawNodeData(),hair.countNode()*sizeof(QVector3D));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(hair.getNodeIndexData(),hair.countIndex()*sizeof(GLuint));

    //TODO
    //头发每个点的法向量
    normalBuf.create();
    normalBuf.bind();
    normalBuf.allocate(hair.getDrawNodeData(),hair.countNode()*sizeof(QVector3D));

    return true;
}

void HairModel::draw(QOpenGLShaderProgram &shaderProgram)
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

void HairModel::update(Env &env, float dt)
{
    hair.update(env,dt);
    updateBuf();
}

void HairModel::rotate(float angle, float x, float y, float z)
{
    mMatrix.rotate(angle,x,y,z);
    hair.setMMatrix(mMatrix);
}


void HairModel::updateBuf()
{
    arrayBuf.bind();
    arrayBuf.allocate(hair.getDrawNodeData(),hair.countNode()*sizeof(QVector3D));

    indexBuf.bind();
    indexBuf.allocate(hair.getNodeIndexData(),hair.countIndex()*sizeof(GLuint));

    normalBuf.bind();
    normalBuf.allocate(hair.getDrawNodeData(),hair.countNode()*sizeof(QVector3D));
}
