#include"openglscene.h"
#include"sphere.h"
#include"math.h"
#include"line.h"
#include"hairmodel.h"
#include"bcurve.h"

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

bool OpenGLScene::initModel()
{
    initCoord();
    initSphereModel();
    initHeadModel();
    initHairModel();

    //    BCurve* curve=new BCurve();
    //    curve->setP1(QVector3D(-2,-1,0));
    //    curve->setP2(QVector3D(1,0,0));
    //    curve->setP3(QVector3D(3,1,0));
    //    curve->setP4(QVector3D(5,3,0));
    //    curve->init();
    //
    //    modelBox.append(curve);

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

    glClearColor(100,100,100,255);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glEnable(GL_POLYGON_OFFSET_POINT);
    glPolygonOffset(0.1,1);

    //    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glEnable(GL_SAMPLE_MASK);
    glSampleCoverage(0,0x0E);
//    glSampleMaski(0, 0x0E);

//    glSampleCoverage();


    initModel();
    initShaders();
    //    initTextures();


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    //    glEnable(GL_CULL_FACE);
}
void OpenGLScene::initShaders()
{
    initPhongShader();
    initKKShader();
}

void OpenGLScene::initPhongShader()
{
    phongShaderV=new QOpenGLShader(QOpenGLShader::Vertex);
    phongShaderF=new QOpenGLShader(QOpenGLShader::Fragment);

    bool flag=true;
    do
    {
        flag=phongShaderV->compileSourceFile("shader/phongV.glsl");
        if(!flag) break;

        flag=phongShaderF->compileSourceFile("shader/phongF.glsl");
        if(!flag) break;
    }
    while(0);

    if(!flag)
    {
        qDebug()<<program.log();
        exit(-1);
    }
}

void OpenGLScene::initKKShader()
{
    kkShaderV=new QOpenGLShader(QOpenGLShader::Vertex);
    kkShaderF=new QOpenGLShader(QOpenGLShader::Fragment);

    bool flag=true;
    do
    {
        flag=kkShaderV->compileSourceFile("shader/kkV.vert");
        if(!flag) break;

        flag=kkShaderF->compileSourceFile("shader/kkF.frag");
        if(!flag) break;

    }
    while(0);

    if(!flag)
    {
        qDebug()<<program.log();
        exit(-1);
    }
}

void OpenGLScene::bindPhongShader(Model *model)
{
    if(curShader!=Phong)
    {
//        program.release();
        bool flag=true;
        do
        {
            if(curShader==KK)
            {
                program.release();
            }

            flag=program.addShader(phongShaderV);
            if(!flag) break;

            flag=program.addShader(phongShaderF);
            if(!flag) break;

            flag=program.link();
            if(!flag) break;

            flag=program.bind();
            if(!flag) break;
        }
        while(0);

        if(!flag)
        {
            qDebug()<<program.log();
            exit(-1);
        }
        curShader=Phong;
    }


    QMatrix3x3 tempMatrix=normalMatrix.toGenericMatrix< 3,3 >();

    program.setUniformValue("pMatrix", projection);
    program.setUniformValue("vMatrix", vMatrix);
    program.setUniformValue("normalMatrix",tempMatrix);
    program.setUniformValue("vLightPosition",lightPos);

    program.setUniformValue("ambientColor", ambiendColor);
    program.setUniformValue("diffuseColor", diffuseColor);
    program.setUniformValue("specularColor", specularColor);

    program.setUniformValue("cameraPos",camera.pos);

    program.setUniformValue("mMatrix", model->getMatrix());
}

void OpenGLScene::bindKKShader(Model *model)
{
    if(curShader!=KK)
    {
//        program.release();
        bool flag=true;
        do
        {
            if(curShader==Phong)
            {
                program.release();
            }


            flag=program.addShader(kkShaderV);
            if(!flag) break;

            flag=program.addShader(kkShaderF);
            if(!flag) break;

            flag=program.link();
            if(!flag) break;

            flag=program.bind();
            if(!flag) break;
        }
        while(0);

        if(!flag)
        {
            qDebug()<<program.log();
            exit(-1);
        }
        curShader=KK;
    }


    QMatrix3x3 tempMatrix=normalMatrix.toGenericMatrix< 3,3 >();

    program.setUniformValue("pMatrix", projection);
    program.setUniformValue("vMatrix", vMatrix);
    program.setUniformValue("normalMatrix",tempMatrix);
    program.setUniformValue("vLightPosition",lightPos);

    program.setUniformValue("ambientColor", ambiendColor);
    program.setUniformValue("diffuseColor", diffuseColor);
    program.setUniformValue("specularColor", specularColor);

    program.setUniformValue("cameraPos",camera.pos);

    program.setUniformValue("mMatrix", model->getMatrix());
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
    Q_UNUSED(event);
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
    releasePos=event->pos();
    auto vec=releasePos-pressPos;
    float len=vec.manhattanLength();

    if(pressPos.x()>releasePos.x())
    {
        len=-len;
    }
    len=len;

    for(int i=0; i<modelBox.size(); i++)
    {
        modelBox[i]->rotate(len,0,1,0);
    }

    pressPos=releasePos;
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

void OpenGLScene::initCoord()
{
    Line* x=new Line();
    x->setP1(QVector3D(0,0,0));
    x->setP2(QVector3D(10,0,0));
    x->init();
    this->modelBox.append(x);

    Line* y=new Line();
    y->setP1(QVector3D(0,0,0));
    y->setP2(QVector3D(0,10,0));
    y->init();
    this->modelBox.append(y);

    Line* z=new Line();
    z->setP1(QVector3D(0,0,0));
    z->setP2(QVector3D(0,0,10));
    z->init();
    this->modelBox.append(z);
}

void OpenGLScene::initSphereModel()
{
    //碰撞球
    Sphere* sphere1=new Sphere();
    sphere1->setTexture("texture/rect.png");
    sphere1->load(QString("model/sphere.obj"));
    sphere1->init();
    env.sphereBox.append(sphere1);
    modelBox.append(sphere1);

    //碰撞球
    Sphere* sphere2=new Sphere();
    sphere2->setTexture("texture/rect.png");
    sphere2->load(QString("model/sphere2.obj"));
    sphere2->init();
    env.sphereBox.append(sphere2);
    modelBox.append(sphere2);

    //碰撞球
    Sphere* sphere3=new Sphere();
    sphere3->setTexture("texture/rect.png");
    sphere3->load(QString("model/sphere3.obj"));
    sphere3->init();
    env.sphereBox.append(sphere3);
    modelBox.append(sphere3);
}

void OpenGLScene::initHeadModel()
{
    //上半身人头模型
    Model* head=new Model();
    head->load(QString("model/head.obj"));
    head->setTexture("texture/rect.png");
    head->init();
    modelBox.append(head);
}

void OpenGLScene::initHairModel()
{
    //头发,由头皮模型生成
    HairModel* scalp=new HairModel();
    scalp->load(QString("model/hairRoot1.obj"));
    scalp->init();
    modelBox.append(scalp);
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

    for(int i=0; i<modelBox.size(); i++)
    {
        if(!modelBox[i]->isHide())
        {
            if(modelBox[i]->getShaderType()==ShaderType::Phong)
            {
                bindPhongShader(modelBox[i]);
            }
            else
            {
                bindKKShader(modelBox[i]);
            }
            modelBox[i]->draw(program);
        }
    }
}

void OpenGLScene::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 50.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);

    vMatrix.setToIdentity();
    vMatrix.lookAt(camera.pos,camera.center,camera.up);

}


