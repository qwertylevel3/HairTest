#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include"stable.h"
#include"model.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE



//顶点数据
struct VertexData
{
    QVector3D pos;
    QVector2D tex;
};

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLScene(QWidget *parent = 0);
    ~OpenGLScene();


    QVector4D getAmbiendColor() const;
    QVector4D getDiffuseColor() const;
    QVector4D getSpecularColor() const;
    QVector3D getLightPos() const;

    void setAmbiendColor(QVector4D vec);
    void setDiffuseColor(QVector4D vec);
    void setSpecularColor(QVector4D vec);
    void setLightPos(QVector3D pos);
protected:
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;

    void initShaders();
    void initTextures();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    QBasicTimer timer;

    QOpenGLTexture *texture;

    QOpenGLShaderProgram program;

    bool initializeModel();
    QVector<Model*> modelBox;

    //鼠标坐标
    QPoint pressPos;
    QPoint releasePos;

    //变换矩阵
    QMatrix4x4 projection;
    QMatrix4x4 mvMatrix;
    QMatrix4x4 normalMatrix;

    //phong 关照
    QVector4D ambiendColor;
    QVector4D diffuseColor;
    QVector4D specularColor;
    QVector3D lightPos;
};

#endif // OPENGLSCENE_H
