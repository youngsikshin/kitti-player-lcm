#ifndef KITTIDATA_H
#define KITTIDATA_H
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QVector>

class KittiData
{
public:
    KittiData();
    KittiData(QString path);
    ~KittiData();
    int get_num_of_frame() { return _times.size(); }

private:
    QFileInfoList _flistLeftImg;
    QFileInfoList _flistRightImg;
    QFileInfoList _flistVelodyne;

    QVector<double> _times;

    QFileInfoList get_filelist(const QString path, const QString name_filter);
    void print_filelist(const QFileInfoList flist);
};

#endif // KITTIDATA_H
