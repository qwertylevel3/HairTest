#ifndef MODEL_H
#define MODEL_H


#include"stable.h"


class Env;

class Model:public QOpenGLFunctions
{
public:
    Model();
    ~Model();
    bool load(const QString &filePath);
    //根据point，indexs,normals初始化buf
    virtual bool init();

    //根据当前buf绘制三角形
    virtual void draw(QOpenGLShaderProgram &shaderProgram);

    //默认无其他行为
    virtual void update(Env& env,float dt);

    //旋转
    virtual void rotate(float angle,float x,float y,float z);

    QString fileName() const
    {
        return m_fileName;
    }
    GLuint countFaces() const
    {
        return pointIndices.size() / 3;
    }

    GLuint countEdges() const
    {
        return edgeIndices.size() / 2;
    }
    GLuint countPoints() const
    {
        return points.size();
    }
    GLuint countIndices() const
    {
        return pointIndices.size();
    }
    GLuint countNormals() const
    {
        return normals.size();
    }

    void* getPointsData()
    {
        return &points[0];
    }
    void* getNormalsData()
    {
        return &normals[0];
    }
    void* getEdgeIndicesData()
    {
        return &edgeIndices[0];
    }
    void* getIndicesData()
    {
        return &pointIndices[0];
    }

    QMatrix4x4 getMatrix()
    {
        return mMatrix;
    }
    void setMatrix(const QMatrix4x4 &matrix);


protected:
    QString m_fileName;//文件名
    QVector<QVector3D> oriPoints;//顶点信息
    QVector<QVector3D> oriNormals;//向量信息
    QVector<QVector3D> points;//变换后的顶点信息
    QVector<QVector3D> normals;//变换后的向量信息
    QVector<GLuint> edgeIndices;//边索引
    QVector<GLuint> pointIndices;//点索引

    QOpenGLBuffer indexBuf{QOpenGLBuffer::IndexBuffer};
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer normalBuf;

    QMatrix4x4 mMatrix;
};
#endif // MODEL_H
