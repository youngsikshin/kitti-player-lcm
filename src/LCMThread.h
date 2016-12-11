#ifndef LCMTHREAD_H
#define LCMTHREAD_H

#include <iostream>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QPixmap>
#include <QVector>
#include <QVector3D>
#include <QReadLocker>
#include <QCanBusFrame>
#include <QtOpenGL>

#include <lcm/lcm-cpp.hpp>
#include "lcmtypes++/bot_core/image_t.hpp"
#include "lcmtypes++/irp_sen/lpc_t.hpp"

using namespace std;

class LCMThread : public QThread
{
    Q_OBJECT

public:
    explicit LCMThread(QObject *parent = 0, QMutex *th_mutex = 0);
    QMutex *mutex;

    QCanBusFrame::TimeStamp timestamp;

    QImage *leftImg;
    QImage *rightImg;
    irp_sen::lpc_t *lpc;

    lcm::LCM _lcm;
    bot_core::image_t left_bot_img;
    bot_core::image_t right_bot_img;

    void set_left_img(QImage *img) { leftImg = img; }
    void set_right_img(QImage *img) { rightImg = img; }
    void set_velodyne(QVector<GLfloat>& velodyneData, QVector<GLfloat>& velodyneReflectance);

    void pub_left_img();
    void pub_right_img();
    void pub_velodyne();

signals:

private:
    void QImage_to_bot_image_t(const QImage* qimg,bot_core::image_t &img_t);
    int QImageFormatToEncoding(const QImage::Format format);
    void run ();

public slots:
    void clear_left_img();
    void clear_right_img();
    void clear_lpc();

};

#endif // LCMTHREAD_H
