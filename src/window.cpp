#include <QtWidgets>
#include <QElapsedTimer>
#include <QTimer>
#include <QVector>

#include "window.h"
#include "ui_window.h"

#include "myglwidget.h"
#include "KittiData.h"

window::window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::window)
{
    ui->setupUi(this);

    strSeq = "00";
    lcmThread = new LCMThread(this, &mutex);

    // Timer
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer->stop();

    init_index();

    strSeq = ui->comboBox->currentText();
    QString seqPath = "/var/data/kitti/dataset/sequences/"+strSeq+"/";

    ui->layerSelector1->setChecked(true);

    kittiData = KittiData(seqPath);
    kittiData.set_velodyne_layer(Layer64);
}

window::~window()
{
//    lcmThread->quit();
//    if(!lcmThread->wait(500)) {
//        lcmThread->terminate();
//        lcmThread->wait();
//    }

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
    strSeq = ui->comboBox->currentText();
    QString seqPath = "/var/data/kitti/dataset/sequences/"+strSeq+"/";
    kittiData = KittiData(seqPath);

    init_index();
}

void window::on_startButton_clicked()
{
    ui->lineEdit->setText("Image path: " + leftImgPath);

    delay_msec = static_cast<int> (kittiData.get_time_diff(i)*1000);
    _timer->start(delay_msec);

    load_data();
}

void window::onTimer()
{
    ui->lineEdit->setText("Image path: " + leftImgPath);

    load_data();
}

void window::on_stopButton_clicked()
{
//    init_index();
    _timer->stop();
}

void window::init_index()
{
    i = 0;
}

void window::load_data()
{
//    QElapsedTimer timer;
//    timer.start();
    if(ui->layerSelector1->isChecked()) kittiData.set_velodyne_layer(Layer64);
    if(ui->layerSelector2->isChecked()) kittiData.set_velodyne_layer(Layer16);

    leftImgPath = kittiData.get_left_img(i);
    rightImgPath = kittiData.get_right_img(i);
    velodynePath = kittiData.get_velodyne(i);

//    delay_msec = static_cast<int> (kittiData.get_time_diff(i)*1000);
//    _timer->start(delay_msec);

    i++;

    kittiData._leftImg = QImage(leftImgPath,"png");
    kittiData._rightImg = QImage(rightImgPath,"png");

    leftImg.convertFromImage(kittiData._leftImg);
    rightImg.convertFromImage(kittiData._rightImg);

    ui->leftcamLabel->setPixmap(leftImg.scaledToWidth(ui->leftcamLabel->width()));
    ui->rightcamLabel->setPixmap(rightImg.scaledToWidth(ui->rightcamLabel->width()));

    // Read velodyne data
    kittiData.read_velodyne(velodynePath);

    this->ui->myGLWidget->_velodyneData.clear();
    this->ui->myGLWidget->_velodyneReflectance.clear();

    if(ui->vlpCheckBox->isChecked()) {

        this->ui->myGLWidget->_velodyneData = kittiData._velodyneData;
        this->ui->myGLWidget->_velodyneReflectance = kittiData._velodyneReflectance;

//        int cnt = 0;
//        for (int i=0; i<kittiData._velodyneLayerData.size(); i++) {
//            this->ui->myGLWidget->_velodyneData << kittiData._velodyneLayerData[i];
//            this->ui->myGLWidget->_velodyneReflectance << kittiData._velodyneLayerReflectance[i];
//        }

    }

    lcmThread->set_left_img(&kittiData._leftImg);
    lcmThread->set_right_img(&kittiData._rightImg);
    lcmThread->set_velodyne(kittiData._velodyneData, kittiData._velodyneReflectance);
    lcmThread->start();

//    _timer->start(delay_msec-timer.elapsed());
    this->ui->myGLWidget->update();

}

void window::clear_left_img123()
{

}

void window::on_stepButton_clicked()
{
    ui->lineEdit->setText("Image path: " + leftImgPath);

    load_data();
}
