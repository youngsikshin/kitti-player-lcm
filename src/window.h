#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QTimer>
#include <QPixmap>
#include "KittiData.h"

namespace Ui {
class window;
}

class window : public QWidget
{
    Q_OBJECT

public:
    explicit window(QWidget *parent = 0);
    ~window();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_startButton_clicked();

    void onTimer();

    void on_stopButton_clicked();

private:
    KittiData kittiData;
    Ui::window *ui;
    QTimer *_timer;

    QString leftImgPath;
    QString rightImgPath;
    QString velodynePath;

    int delay_msec;
    QPixmap leftImg;
    QPixmap rightImg;
    int i=0;

    void init_index();
    void load_data();

};

#endif // WINDOW_H
