#include <QMutexLocker>
#include <QDebug>
#include <algorithm>
#include <chrono>

#include "LCMThread.h"

using namespace std;

LCMThread::LCMThread(QObject *parent, QMutex *th_mutex) :
    QThread(parent), mutex(th_mutex)
{
    leftImg = new QImage();
    rightImg = new QImage();

    timestamp = QCanBusFrame::TimeStamp(0, 100);

    if(!_lcm.good())
        return;
}

void LCMThread::pub_left_img()
{
    QImage_to_bot_image_t(leftImg, left_bot_img);
    _lcm.publish("LEFT_IMG_T",&left_bot_img);
    clear_left_img();
}

void LCMThread::pub_right_img()
{
    QImage_to_bot_image_t(rightImg, right_bot_img);
    _lcm.publish("RIGHT_IMG_T",&right_bot_img);
    clear_right_img();
}

void LCMThread::clear_left_img()
{
    left_bot_img.data.clear();
}

void LCMThread::clear_right_img()
{
    right_bot_img.data.clear();
}

void LCMThread::QImage_to_bot_image_t(const QImage* qimg,bot_core::image_t &img_t)
{

    std::chrono::system_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::system_clock::duration duration_now = now.time_since_epoch();

    img_t.utime = std::chrono::duration_cast<std::chrono::microseconds>(duration_now).count();

    img_t.width = qimg->width();
    img_t.height = qimg->height();
    img_t.row_stride = 1*qimg->width();

    img_t.pixelformat = QImageFormatToEncoding(qimg->format());
    img_t.size = qimg->width()*qimg->height();

//    img_t.data.assign(qimg->bits(), qimg->bits()+qimg->byteCount());
    for (int i=0; i<qimg->height(); i++) {
        img_t.data.insert(img_t.data.begin()+i*qimg->width(), qimg->scanLine(i), qimg->scanLine(i)+qimg->width());
    }

//    qDebug() << qimg->byteCount() << "," << qimg->bytesPerLine() << ", " << qimg->width() << ", " << img_t.height << ", " << qimg->size() << ", " << img_t.data.size() << ", " << img_t.size;

    img_t.nmetadata = 0;
//    img_t.metadata;
}

int LCMThread::QImageFormatToEncoding(const QImage::Format format)
{
//    qDebug() << format;
    switch (format) {
    case QImage::Format_Invalid:
        return bot_core::image_t::PIXEL_FORMAT_INVALID;
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    case QImage::Format_Indexed8:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_RGB16:
    case QImage::Format_ARGB8565_Premultiplied:
    case QImage::Format_RGB666:
    case QImage::Format_ARGB8555_Premultiplied:
    case QImage::Format_RGB888:
    case QImage::Format_RGB444:
    case QImage::Format_ARGB4444_Premultiplied:
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
    case QImage::Format_RGBA8888_Premultiplied:
    case QImage::Format_BGR30:
    case QImage::Format_A2BGR30_Premultiplied:
    case QImage::Format_RGB30:
    case QImage::Format_A2RGB30_Premultiplied:
    case QImage::Format_Alpha8:
    case QImage::Format_Grayscale8:
        return bot_core::image_t::PIXEL_FORMAT_GRAY;
    }
}

void LCMThread::run()
{
    pub_left_img();
    pub_right_img();
}
