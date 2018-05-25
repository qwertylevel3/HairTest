#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"stable.h"
#include"openglscene.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void changeAmbiendColor();
    void changeDiffuseColor();
    void changeSpecularColor();
    void changeLightPos();
    void changeWind();

    void keyPressEvent(QKeyEvent *event);
private:
    QVector4D getAmbiendColor();
    QVector4D getDiffuseColor();
    QVector4D getSpecularColor();
    QVector3D getLightPos();
    QVector4D getWind();
    Ui::MainWindow *ui;
    OpenGLScene* scene;
};

#endif // MAINWINDOW_H
