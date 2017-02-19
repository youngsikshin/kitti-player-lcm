#ifndef KITTIDATA_H
#define KITTIDATA_H
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QVector>
#include <QtOpenGL>
#include <QMatrix4x4>
#include <vector>

enum VelodyneLayer {
    Layer16,
    Layer64
};

class KittiData
{
public:
    KittiData();
    KittiData(QString path, QString gtFname);
    ~KittiData();

    int get_velodyne_layer() { return velodyneLayer; }
    void set_velodyne_layer(VelodyneLayer layer) { velodyneLayer = layer; }

    int get_num_of_frame() { return _times.size(); }
    QString get_left_img(int i) { return _flistLeftImg.at(i).absoluteFilePath(); }
    QString get_right_img(int i) { return _flistRightImg.at(i).absoluteFilePath(); }
    QString get_velodyne(int i) { return _flistVelodyne.at(i).absoluteFilePath();}
    QMatrix4x4 get_poses(int i) { return _poses[i]; }
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
    QString _gtFname;

    QFileInfoList _flistLeftImg;
    QFileInfoList _flistRightImg;
    QFileInfoList _flistVelodyne;

    VelodyneLayer velodyneLayer;

    QVector<double> _times;
    QVector<QMatrix4x4> _poses;

    QFileInfoList get_filelist(const QString path, const QString name_filter);
    void print_filelist(const QFileInfoList flist);

    void insert_QVector(QVector<GLfloat>& dst, const QVector<GLfloat>& src);
};

#endif // KITTIDATA_H
