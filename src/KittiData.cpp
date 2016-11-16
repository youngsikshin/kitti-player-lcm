#include <iostream>

#include <QDir>
#include <QTextStream>
#include <QDataStream>
#include "KittiData.h"

KittiData::KittiData()
{

}

KittiData::KittiData(QString path)
{
    _leftImgPath = path+"image_0";
    _rightImgPath = path+"image_1";
    _velodynePath = path+"velodyne";

    _flistLeftImg = get_filelist(_leftImgPath, "*.png");
    _flistRightImg = get_filelist(_rightImgPath,"*.png");
    _flistVelodyne = get_filelist(_velodynePath,"*.bin");

    QFile _ftimes(path+"times.txt");
    if (!_ftimes.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&_ftimes);
    while (!in.atEnd()) {
        QString line = in.readLine();
        _times.push_back(line.toDouble());
    }
}

KittiData::~KittiData()
{

}

QFileInfoList KittiData::get_filelist(const QString path, const QString name_filter)
{
    QDir dir(path);
    QStringList name_filters;
    name_filters << name_filter;

    QFileInfoList filelist = dir.entryInfoList(name_filters, QDir::NoDotAndDotDot|QDir::AllDirs|QDir::Files);

    return filelist;
}

void KittiData::print_filelist(const QFileInfoList flist)
{
    std::cout << "[KittiData]\t" << flist.size() << std::endl;
    for (int i = 0; i < flist.size(); i++)
    {
        QFileInfo fi = flist.at(i);
        if (fi.isFile())
            std::cout << fi.fileName().toStdString() << std::endl;
    }
}

void KittiData::read_velodyne(QString fname)
{
    QFile velodyne_file(fname);
    if (!velodyne_file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&velodyne_file);
    while(!in.atEnd()) {
        in.setByteOrder(QDataStream::LittleEndian);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        float x, y, z, r;
        in >> x >> y >> z >> r;
//        std::cout << x << ", " << y << ", " << z << ", " << r << std::endl;
    }
}
