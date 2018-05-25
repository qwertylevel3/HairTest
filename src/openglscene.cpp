#include "openglscene.h"
#include"sphere.h"
#include"math.h"
#include"line.h"
#include "hairmodel.h"


OpenGLScene::OpenGLScene(QWidget *parent)
    : QOpenGLWidget(parent)
{
    timer.start(12,this);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    this->setFormat(format);
    this->setEnabled(true);
}

OpenGLScene::~OpenGLScene()
{
}

void OpenGLScene::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    update();
}

bool OpenGLScene::initializeModel()
{
    Sphere* model=new Sphere();
    model->load(QString("model/sphere.obj"));
    model->init();
    env.sphereBox.append(model);
    modelBox.append(model);

    HairModel* scalp=new HairModel();
    scalp->load(QString("model/hairRoot1.obj"));
    scalp->init();
    modelBox.append(scalp);

//    Line* line=new Line();
//    line->init();
//    line->setP1(QVector3D(0,0,0));
//    line->setP1(QVector3D(5,5,5));
//    modelBox.append(line);

    return true;
}

QVector3D OpenGLScene::getLightPos() const
{
    return lightPos;
}

QVector4D OpenGLScene::getSpecularColor() const
{
    return specularColor;
}

QVector4D OpenGLScene::getDiffuseColor() const
{
    return diffuseColor;
}

QVector4D OpenGLScene::getAmbiendColor() const
{
    return ambiendColor;
}

void OpenGLScene::initializeGL()
{
    initializeOpenGLFunctions();

    initializeModel();
    initShaders();
//    initTextures();

    vMatrix.translate(0.0, 0.0, -5.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
void OpenGLScene::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shader/vshader.glsl"))
    {
        qDebug()<<program.log();
        exit(-1);
    }

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shader/fshader.glsl"))
    {
        qDebug()<<program.log();
        exit(-1);
    }

    // Link shader pipeline
    if (!program.link())
    {
        qDebug()<<program.log();
        exit(-1);
    }

    // Bind shader pipeline for use
    if (!program.bind())
    {
        qDebug()<<program.log();
        exit(-1);
    }
}

void OpenGLScene::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage("test.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLScene::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
        int index=event->key()-0x30;
        changeModelHidden(index);
        break;
    }
}

void OpenGLScene::mousePressEvent(QMouseEvent *event)
{
    pressPos=event->pos();
    if(event->button()==Qt::RightButton)
    {
        mouseFlag=ROTATECAMERA;
//        mouseMoveEvent(event);
    }
    else if(event->button()==Qt::MidButton)
    {
        mouseFlag=SCALE;
    }
    else if(event->button()==Qt::LeftButton)
    {
        mouseFlag=ROTATEMODEL;
    }
}

void OpenGLScene::mouseReleaseEvent(QMouseEvent *event)
{
    mouseFlag=NORMAL;
}

void OpenGLScene::mouseMoveEvent(QMouseEvent *event)
{
    if(mouseFlag==ROTATECAMERA)
    {
        rotateCamera(event);
    }
    else if(mouseFlag==ROTATEMODEL)
    {
        rotateModel(event);
    }

}

void OpenGLScene::wheelEvent(QWheelEvent *event)
{
    float x=sin(float(angle)*3.14/180.0);
    float z=cos(float(angle)*3.14/180.0);

    QVector3D vec(-x,0,-z);
    vec.normalize();
    vec*=event->delta()/200.0;

    camera.pos.setX(camera.pos.x()+vec.x());
    camera.pos.setZ(camera.pos.z()+vec.z());

    vMatrix.setToIdentity();
    vMatrix.lookAt(camera.pos,camera.center,camera.up);
}

void OpenGLScene::rotateCamera(QMouseEvent *event)
{
    releasePos=event->pos();
    auto vec=releasePos-pressPos;
    int len=vec.manhattanLength();

    if(pressPos.x()>releasePos.x())
    {
        len=-len;
    }
    angle+=len;


    float x=sin(float(angle)*3.14/180.0);
    float z=cos(float(angle)*3.14/180.0);
    float dis=camera.pos.length();

    QVector3D vec2(-x,0,-z);
    vec2.normalize();
    vec2*=dis;

    camera.pos.setX(vec2.x());
    camera.pos.setZ(vec2.z());

    vMatrix.setToIdentity();
    vMatrix.lookAt(camera.pos,camera.center,camera.up);

    normalMatrix.rotate(len,0,1,0);

    pressPos=releasePos;

}

void OpenGLScene::rotateModel(QMouseEvent *event)
{
    for(int i=0; i<modelBox.size(); i++)
    {
        releasePos=event->pos();
        auto vec=releasePos-pressPos;
        float len=vec.manhattanLength();

        if(pressPos.x()>releasePos.x())
        {
            len=-len;
        }
        len=len/1000;
        modelRotateAngle+=len;

        modelBox[i]->rotate(modelRotateAngle,0,1,0);
    }
}

void OpenGLScene::changeModelHidden(int index)
{
    qDebug()<<index;
    if(index<0 || index>=modelBox.size())
    {
        return ;
    }
    bool flag=modelBox[index]->isHide();
    modelBox[index]->setHide(!flag);
}

void OpenGLScene::setAmbiendColor(QVector4D vec)
{
    ambiendColor=vec;
}

void OpenGLScene::setDiffuseColor(QVector4D vec)
{
    diffuseColor=vec;
}

void OpenGLScene::setSpecularColor(QVector4D vec)
{
    specularColor=vec;
}

void OpenGLScene::setLightPos(QVector3D pos)
{
    lightPos=pos;
}

void OpenGLScene::setWind(QVector4D w)
{
    env.wind=w;
}

void OpenGLScene::update()
{
    //时间间隔
    float delta=0.03;
    for(int i=0; i<modelBox.size(); i++)
    {
        modelBox[i]->update(env,delta);
    }
    QOpenGLWidget::update();
}

void OpenGLScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绑定纹理
//    texture->bind();
//    program.setUniformValue("texture", 0);


    QMatrix3x3 tempMatrix=normalMatrix.toGenericMatrix< 3,3 >();

    program.setUniformValue("mvpMatrix", projection * vMatrix);
    program.setUniformValue("vMatrix", vMatrix);
    program.setUniformValue("normalMatrix",tempMatrix);
    program.setUniformValue("vLightPosition",lightPos);

    program.setUniformValue("ambientColor", ambiendColor);
    program.setUniformValue("diffuseColor", diffuseColor);
    program.setUniformValue("specularColor", specularColor);


    for(int i=0; i<modelBox.size(); i++)
    {
        program.setUniformValue("mMatrix", modelBox[i]->getMatrix());
        if(!modelBox[i]->isHide())
        {
            modelBox[i]->draw(program);
        }
    }
}

void OpenGLScene::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 20.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}


