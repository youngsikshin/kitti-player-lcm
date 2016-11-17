#include <QtWidgets>
#include <QElapsedTimer>
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
    QString seqPath = "/var/data/kitti/dataset/sequences/00/";
    kittiData = KittiData(seqPath);

    ui->lineEdit->setText("Image path: " + leftImgPath);

    load_data();
}

void window::onTimer()
{
    ui->lineEdit->setText("Image path: " + leftImgPath);

    load_data();
}

void window::on_stopButton_clicked()
{
    init_index();
    _timer->stop();
}

void window::init_index()
{
    i = 0;
}

void window::load_data()
{
    QElapsedTimer timer;
    timer.start();
    leftImgPath = kittiData.get_left_img(i);
    rightImgPath = kittiData.get_right_img(i);
    velodynePath = kittiData.get_velodyne(i);

    delay_msec = static_cast<int> (kittiData.get_time_diff(i)*1000);

    i++;

    leftImg = QPixmap(leftImgPath);
    rightImg = QPixmap(rightImgPath);

    ui->leftcamLabel->setPixmap(leftImg.scaledToWidth(ui->leftcamLabel->width()));
    ui->rightcamLabel->setPixmap(rightImg.scaledToWidth(ui->rightcamLabel->width()));

    kittiData.read_velodyne(velodynePath);
    this->ui->myGLWidget->_velodyneData = kittiData._velodyneData;
    this->ui->myGLWidget->_velodyneReflectance = kittiData._velodyneReflectance;

    _timer->start(delay_msec-timer.elapsed());
    this->ui->myGLWidget->update();
}
