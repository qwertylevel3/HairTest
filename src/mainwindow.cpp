#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglscene.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene=new OpenGLScene(this);



    this->setCentralWidget(scene);


    connect(ui->ambientRSlider,SIGNAL(valueChanged(int)),this,SLOT(changeAmbiendColor()));
    connect(ui->ambientGSlider,SIGNAL(valueChanged(int)),this,SLOT(changeAmbiendColor()));
    connect(ui->ambientBSlider,SIGNAL(valueChanged(int)),this,SLOT(changeAmbiendColor()));
    connect(ui->ambientASlider,SIGNAL(valueChanged(int)),this,SLOT(changeAmbiendColor()));

    connect(ui->diffuseRSlider,SIGNAL(valueChanged(int)),this,SLOT(changeDiffuseColor()));
    connect(ui->diffuseGSlider,SIGNAL(valueChanged(int)),this,SLOT(changeDiffuseColor()));
    connect(ui->diffuseBSlider,SIGNAL(valueChanged(int)),this,SLOT(changeDiffuseColor()));
    connect(ui->diffuseASlider,SIGNAL(valueChanged(int)),this,SLOT(changeDiffuseColor()));

    connect(ui->specularRSlider,SIGNAL(valueChanged(int)),this,SLOT(changeSpecularColor()));
    connect(ui->specularGSlider,SIGNAL(valueChanged(int)),this,SLOT(changeSpecularColor()));
    connect(ui->specularBSlider,SIGNAL(valueChanged(int)),this,SLOT(changeSpecularColor()));
    connect(ui->specularASlider,SIGNAL(valueChanged(int)),this,SLOT(changeSpecularColor()));

    connect(ui->lightXSlider,SIGNAL(valueChanged(int)),this,SLOT(changeLightPos()));
    connect(ui->lightYSlider,SIGNAL(valueChanged(int)),this,SLOT(changeLightPos()));
    connect(ui->lightZSlider,SIGNAL(valueChanged(int)),this,SLOT(changeLightPos()));

    connect(ui->windXSlider,SIGNAL(valueChanged(int)),this,SLOT(changeWind()));
    connect(ui->windYSlider,SIGNAL(valueChanged(int)),this,SLOT(changeWind()));
    connect(ui->windZSlider,SIGNAL(valueChanged(int)),this,SLOT(changeWind()));


    //init
    changeAmbiendColor();
    changeDiffuseColor();
    changeSpecularColor();
    changeLightPos();
    changeWind();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeAmbiendColor()
{
    scene->setAmbiendColor(this->getAmbiendColor());
}

void MainWindow::changeDiffuseColor()
{
    scene->setDiffuseColor(this->getDiffuseColor());
}

void MainWindow::changeSpecularColor()
{
    scene->setSpecularColor(this->getSpecularColor());
}

void MainWindow::changeLightPos()
{
    scene->setLightPos(this->getLightPos());
}

void MainWindow::changeWind()
{
    scene->setWind(this->getWind());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    scene->keyPressEvent(event);
}

QVector4D MainWindow::getAmbiendColor()
{
    float tr=ui->ambientRSlider->value();
    float tg=ui->ambientGSlider->value();
    float tb=ui->ambientBSlider->value();
    float ta=ui->ambientASlider->value();

    float r=tr/255.0;
    float g=tg/255.0;
    float b=tb/255.0;
    float a=ta/255.0;

    return QVector4D(r,g,b,a);
}

QVector4D MainWindow::getDiffuseColor()
{
    float tr=ui->diffuseRSlider->value();
    float tg=ui->diffuseGSlider->value();
    float tb=ui->diffuseBSlider->value();
    float ta=ui->diffuseASlider->value();

    float r=tr/255.0;
    float g=tg/255.0;
    float b=tb/255.0;
    float a=ta/255.0;

    return QVector4D(r,g,b,a);
}

QVector4D MainWindow::getSpecularColor()
{
    float tr=ui->specularRSlider->value();
    float tg=ui->specularGSlider->value();
    float tb=ui->specularBSlider->value();
    float ta=ui->specularASlider->value();

    float r=tr/255.0;
    float g=tg/255.0;
    float b=tb/255.0;
    float a=ta/255.0;

    return QVector4D(r,g,b,a);

}

QVector3D MainWindow::getLightPos()
{
    float x=float(ui->lightXSlider->value())/25.0;
    float y=float(ui->lightYSlider->value())/25.0;
    float z=float(ui->lightZSlider->value())/25.0;

    return QVector3D(x,y,z);
}

QVector4D MainWindow::getWind()
{
    float x=float(ui->windXSlider->value())/255.0;
    float y=float(ui->windYSlider->value())/255.0;
    float z=float(ui->windZSlider->value())/255.0;
    float w=float(ui->intensitySlider->value());

    return QVector4D(x,y,z,w);
}
