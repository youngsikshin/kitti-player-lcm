#include <QtWidgets>
#include <QtOpenGL>
#include <QVector3D>
#include <QMatrix4x4>

#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    yaw = -90.f;
    pitch = 0;

    cameraPosition = QVector3D(0, 0, 25);
    cameraFront = QVector3D(0, 0, -1);
    cameraUpDirection = QVector3D(0, 1, 0);

    setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget()
{
}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/resources/vertexShader.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/resources/fragmentShader.fsh");
    shaderProgram.link();

    shaderProgramColor.addShaderFromSourceFile(QGLShader::Vertex, ":/resources/vertexShaderColor.vsh");
    shaderProgramColor.addShaderFromSourceFile(QGLShader::Fragment, ":/resources/fragmentShaderColor.fsh");
    shaderProgramColor.link();

    axes << QVector3D(0, 0, 0) << QVector3D(1, 0, 0)
         << QVector3D(0, 0, 0) << QVector3D(0, 1, 0)
         << QVector3D(0, 0, 0) << QVector3D(0, 0, 1);

    axes_color << QVector3D(1, 0, 0) << QVector3D(1, 0, 0)
               << QVector3D(0, 1, 0) << QVector3D(0, 1, 0)
               << QVector3D(0, 0, 1) << QVector3D(0, 0, 1);

    int max_size = 10;
    for (int i=0; i<=max_size; i++) {
        grid.push_back(QVector3D(-max_size, i, 0)); grid.push_back(QVector3D(max_size, i, 0));
        grid.push_back(QVector3D(-max_size, -i, 0)); grid.push_back(QVector3D(max_size, -i, 0));
        grid.push_back(QVector3D(i, -max_size, 0)); grid.push_back(QVector3D(i, max_size, 0));
        grid.push_back(QVector3D(-i, -max_size, 0)); grid.push_back(QVector3D(-i, max_size, 0));
    }

}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;

    mMatrix.setToIdentity();

    vMatrix.lookAt(cameraPosition, cameraPosition+cameraFront, cameraUpDirection);
    QMatrix4x4 T = pMatrix * vMatrix * mMatrix;

    glLineWidth(1);
    drawShader(shaderProgram, GL_LINES, T, grid, Qt::darkGray);
    glLineWidth(3);
    drawShader(shaderProgramColor, GL_LINES, T, axes, axes_color);

    drawShader(shaderProgramColor, GL_POINTS, T, _velodyneData, _velodyneReflectance, 1);

//    glFlush();
}

void MyGLWidget::drawShader(QGLShaderProgram &shader, GLenum type, QMatrix4x4 T, QVector<QVector3D> data, Qt::GlobalColor color)
{
    shader.bind();
    shader.setUniformValue("color", QColor(color));
    shader.setAttributeArray("vertex", data.constData());
    shader.enableAttributeArray("vertex");
    glDrawArrays(type, 0, data.size());
    shader.disableAttributeArray("vertex");
    shader.setUniformValue("mvpMatrix", T);
    shader.release();
}

void MyGLWidget::drawShader(QGLShaderProgram &shader, GLenum type, QMatrix4x4 T, QVector <GLfloat> data, QVector <GLfloat> data_color, int stride)
{
    shader.bind();
    shader.setAttributeArray("vertex", data.constData(), 3, 3*stride*sizeof(GLfloat));
    shader.enableAttributeArray("vertex");
    shader.setAttributeArray("color", data_color.constData(), 3, 3*stride*sizeof(GLfloat));
    shader.enableAttributeArray("color");
    if (stride==0) stride = 1;
    glDrawArrays(type, 0, data.size()/(3*stride));
    shader.disableAttributeArray("color");
    shader.disableAttributeArray("vertex");
    shader.setUniformValue("mvpMatrix", T);
    shader.release();
}

void MyGLWidget::drawShader(QGLShaderProgram &shader, GLenum type, QMatrix4x4 T, QVector<QVector3D> data, QVector<QVector3D> data_color)
{
    shader.bind();
    shader.setAttributeArray("vertex", data.constData(), sizeof(QVector3D));
    shader.enableAttributeArray("vertex");
    shader.setAttributeArray("color", data_color.constData(), sizeof(QVector3D));
    shader.enableAttributeArray("color");
    glDrawArrays(type, 0, data.size());
    shader.disableAttributeArray("color");
    shader.disableAttributeArray("vertex");
    shader.setUniformValue("mvpMatrix", T);
    shader.release();
}

void MyGLWidget::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);   //<--- add
    glLoadIdentity();              //<--- add
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();

    glPushMatrix();
    glBegin(GL_POINT);
    glColor3d(1, 1, 0);
    glVertex3f(0, 0, 0);
    glEnd();
    glPopMatrix();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = event->x() - lastMousePosition.x();
    float dy = event->y() - lastMousePosition.y();
    float sensitivity = 0.1;

    float alpha;
    QVector3D pt_xy;
    QVector3D tmp_cam_pos;

    if (abs(cameraFront.z()) > 0.000005) {
        alpha = -cameraPosition.z() / cameraFront.z();
        pt_xy = cameraPosition + alpha*cameraFront;
    }
    else {
        pt_xy = cameraPosition;
    }

    tmp_cam_pos = cameraPosition - pt_xy;

    if (event->buttons() & Qt::LeftButton) { // translation motion
        QVector3D left = QVector3D::normal(cameraUpDirection, cameraFront);
        cameraPosition = cameraPosition - (cameraPosition.z()*0.01*sensitivity)*dx*cameraUpDirection;
        cameraPosition = cameraPosition + (cameraPosition.z()*0.01*sensitivity)*dx*left;
        updateGL();
    }
    if (event->buttons() & Qt::RightButton) {
//        deltaX *= sensitivity;
//        deltaY *= sensitivity;

//        yaw += deltaX;
//        pitch += deltaY;

//        if(pitch > 89.0f)
//            pitch = 89.0f;
//        if(pitch < -89.0f)
//            pitch = -89.0f;

//        QVector3D front;
//        front.setX(qCos(qDegreesToRadians(yaw))*qCos(qDegreesToRadians(pitch)));
//        front.setY(qSin(qDegreesToRadians(pitch)));
//        front.setZ(qSin(qDegreesToRadians(yaw))*qCos(qDegreesToRadians(pitch)));
//        cameraFront = front.normalized();
        QMatrix4x4 rotation;
        rotation.setToIdentity();

//        if (cameraFront.z() != 0) {
//            alpha = -cameraPosition.z() / cameraFront.z();
//            pt_xy = cameraPosition + alpha*cameraFront;
//        }
//        else {
//            pt_xy = cameraPosition;
//        }

//        tmp_cam_pos = cameraPosition - pt_xy;

        QVector3D right = QVector3D::normal(cameraFront, cameraUpDirection);

        dx *= sensitivity;
        dy *= sensitivity;

        yaw += dx;
        pitch += dy;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        rotation.rotate(dy, right);
        rotation.rotate(-dx, 0, 0, 1);


        cameraPosition = rotation*tmp_cam_pos + pt_xy;
        cameraFront = rotation*cameraFront;
        cameraUpDirection = rotation*cameraUpDirection;

        updateGL();
    }

    lastMousePosition = event->pos();
    emit mouseEvent();

    event->accept();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = (float)event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            cameraPosition -= 1.8 * cameraFront;
        } else if (delta > 0) {
            cameraPosition += 0.8 * cameraFront;
        }
        updateGL();
    }
    emit mouseEvent();

    event->accept();
}

void MyGLWidget::draw()
{
//    qglColor(Qt::red);
//    glBegin(GL_QUADS);
//        glNormal3f(0,0,-1);
//        glVertex3f(-1,-1,0);
//        glVertex3f(-1,1,0);
//        glVertex3f(1,1,0);
//        glVertex3f(1,-1,0);

//    glEnd();
//    glBegin(GL_TRIANGLES);
//        glNormal3f(0,-1,0.707);
//        glVertex3f(-1,-1,0);
//        glVertex3f(1,-1,0);
//        glVertex3f(0,0,1.2);
//    glEnd();
//    glBegin(GL_TRIANGLES);
//        glNormal3f(1,0, 0.707);
//        glVertex3f(1,-1,0);
//        glVertex3f(1,1,0);
//        glVertex3f(0,0,1.2);
//    glEnd();
//    glBegin(GL_TRIANGLES);
//        glNormal3f(0,1,0.707);
//        glVertex3f(1,1,0);
//        glVertex3f(-1,1,0);
//        glVertex3f(0,0,1.2);
//    glEnd();
//    glBegin(GL_TRIANGLES);
//        glNormal3f(-1,0,0.707);
//        glVertex3f(-1,1,0);
//        glVertex3f(-1,-1,0);
//        glVertex3f(0,0,1.2);
//    glEnd();
}
