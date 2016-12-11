#ifndef KITTIDATA_H
#define KITTIDATA_H
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QVector>
#include <QtOpenGL>
#include <vector>

class KittiData
{
public:
    KittiData();
    KittiData(QString path);
    ~KittiData();
    int get_num_of_frame() { return _times.size(); }
    QString get_left_img(int i) { return _flistLeftImg.at(i).absoluteFilePath(); }
    QString get_right_img(int i) { return _flistRightImg.at(i).absoluteFilePath(); }
    QString get_velodyne(int i) { return _flistVelodyne.at(i).absoluteFilePath();}
    double get_time_diff(int i) { return _times[i+1]-_times[i]; }

    void read_velodyne(QString fname);

    QVector<GLfloat> _velodyneData;
    QVector<GLfloat> _velodyneReflectance;

    QImage _leftImg;
    QImage _rightImg;

private:
    QString _leftImgPath;
    QString _rightImgPath;
    QString _velodynePath;

    QFileInfoList _flistLeftImg;
    QFileInfoList _flistRightImg;
    QFileInfoList _flistVelodyne;

    QVector<double> _times;

    QFileInfoList get_filelist(const QString path, const QString name_filter);
    void print_filelist(const QFileInfoList flist);
};

#endif // KITTIDATA_H
