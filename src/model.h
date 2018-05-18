#ifndef MODEL_H
#define MODEL_H


#include"stable.h"

class Model:public QOpenGLFunctions
{
public:
    Model();
    ~Model();
    bool load(const QString &filePath);
    virtual bool init();
    virtual void draw(QOpenGLShaderProgram &shaderProgram);

    QString fileName() const { return m_fileName; }
    GLuint countFaces() const { return pointIndices.size() / 3; }

    GLuint countEdges() const { return edgeIndices.size() / 2; }
    GLuint countPoints() const { return points.size(); }
    GLuint countIndices() const { return pointIndices.size();}
    GLuint countNormals() const{return normals.size();}

    void* getPointsData(){return &points[0];}
    void* getNormalsData() {return &normals[0];}
    void* getEdgeIndicesData(){return &edgeIndices[0];}
    void* getIndicesData(){return &pointIndices[0];}
protected:

    QString m_fileName;//文件名
    QVector<QVector3D> points;//顶点信息
    QVector<QVector3D> normals;//向量信息
    QVector<GLuint> edgeIndices;//边索引
    QVector<GLuint> pointIndices;//点索引

    QOpenGLBuffer indexBuf{QOpenGLBuffer::IndexBuffer};
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer normalBuf;

};
#endif // MODEL_H
