#include <QtWidgets>
#include <QTimer>
#include "window.h"
#include "ui_window.h"

#include "myglwidget.h"
#include "KittiData.h"

window::window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::window)
{
    ui->setupUi(this);

    // Timer
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer->stop();

    connect(ui->myGLWidget, SIGNAL(xRotationChanged(int)), ui->rotXSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)), ui->rotYSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)), ui->rotZSlider, SLOT(setValue(int)));

}

window::~window()
{
    delete ui;
}

void window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void window::on_comboBox_currentIndexChanged(int index)
{
    ui->lineEdit->setText("Sequence " + ui->comboBox->currentText() + ". ");
}

void window::on_startButton_clicked()
{
    KittiData(QString("/var/data/kitti/dataset/sequences/00/"));
}

void window::onTimer()
{
    ui->lineEdit->setText("onTimer");
}
