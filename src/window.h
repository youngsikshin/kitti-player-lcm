#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QTimer>

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

private:
    Ui::window *ui;
    QTimer *_timer;
};

#endif // WINDOW_H
