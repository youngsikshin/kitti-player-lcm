#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>

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

private:
    Ui::window *ui;
};

#endif // WINDOW_H
