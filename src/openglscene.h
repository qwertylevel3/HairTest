#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include"stable.h"
#include"model.h"
#include"sphere.h"
#include"env.h"


QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

struct Camera
{
    QVector3D pos{0.0,0.0,-10.0};
    QVector3D center{0.0,0.0,0.0};
    QVector3D up{0.0,1.0,0.0};
};

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    enum MouseFlag
    {
        NORMAL,SCALE,ROTATECAMERA,ROTATEMODEL
    };

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
    void setWind(QVector4D w);
    void update();

    void keyPressEvent(QKeyEvent *event);
protected:
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;

    void initShaders();
    void initTextures();


    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void rotateCamera(QMouseEvent *event);
    void rotateModel(QMouseEvent* event);

    void changeModelHidden(int index);


    void initSphereModel();
    void initHeadModel();
    void initHairModel();

    QBasicTimer timer;

    QOpenGLTexture *texture;

    QOpenGLShaderProgram program;

    bool initModel();
    QVector<Model*> modelBox;

    //鼠标坐标
    QPoint pressPos;
    QPoint releasePos;
    MouseFlag mouseFlag{NORMAL};


    //变换矩阵
    QMatrix4x4 projection;
    QMatrix4x4 vMatrix;
    QMatrix4x4 normalMatrix;

    //phong 关照
    QVector4D ambiendColor;
    QVector4D diffuseColor;
    QVector4D specularColor;
    QVector3D lightPos;

    //场景参数
    Env env;

    //camera
    Camera camera;
    int angle{0};
};

#endif // OPENGLSCENE_H
