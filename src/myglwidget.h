#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QVector3D>
#include <QGLShaderProgram>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
//    void keyPressEvent(QKeyEvent *event);

    void drawShader (QGLShaderProgram &shader, GLenum type, QMatrix4x4 T, QVector <QVector3D> data, Qt::GlobalColor color);
    void drawShader (QGLShaderProgram &shader, GLenum type, QMatrix4x4 T, QVector <QVector3D> data, QVector <QVector3D> data_color);
//    void drawShaderBuffer (QGLShaderProgram &shader, GLenum type, QMatrix4x4 T);

    void drawShader(QGLShaderProgram &shader, GLenum type, QMatrix4x4 T, QVector<GLfloat> data, QVector<GLfloat> data_color, int stride);


public:
    QVector <QVector3D> axes;
    QVector <QVector3D> axes_color;
    QVector <QVector3D> grid;

    QGLShaderProgram shaderProgram;
    QGLShaderProgram shaderProgramColor;

    QVector<GLfloat> _velodyneData;
    QVector<GLfloat> _velodyneReflectance;

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    // signaling rotation from mouse movement
    void mouseEvent();
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    void draw();

    int xRot;
    int yRot;
    int zRot;

    QMatrix4x4 pMatrix;
    QPoint lastMousePosition;
    float yaw;
    float pitch;

    QVector3D cameraPosition;
    QVector3D cameraFront;
    QVector3D cameraUpDirection;

};

#endif // MYGLWIDGET_H
