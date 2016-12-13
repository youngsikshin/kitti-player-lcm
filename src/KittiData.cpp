#include <iostream>

#include <QDir>
#include <QTextStream>
#include <QDataStream>

#include <QDebug>
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
//    std::cout << "[KittiData]\t" << flist.size() << std::endl;
    for (int i = 0; i < flist.size(); i++)
    {
        QFileInfo fi = flist.at(i);
        if (fi.isFile()) {
//            std::cout << fi.fileName().toStdString() << std::endl;
        }
    }
}

void KittiData::read_velodyne(QString fname)
{
    QFile velodyne_file(fname);
    if (!velodyne_file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&velodyne_file);
    _velodyneData.clear();
    _velodyneReflectance.clear();

    int cnt=0;
    while(!in.atEnd()) {
        in.setByteOrder(QDataStream::LittleEndian);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        double x, y, z, r;
        double angle = atan2(static_cast<double> (z), static_cast<double> (sqrt(x*x+y*y)))*180.0/M_PI;
//        std::cout << angle << std::endl;
//        std::cout << floor(angle*10+0.5)/10 << std::endl;
        in >> x >> y >> z >> r;

        _velodyneData.push_back(x);
        _velodyneData.push_back(y);
        _velodyneData.push_back(z);
        _velodyneReflectance.push_back(r);
        _velodyneReflectance.push_back(r);
        _velodyneReflectance.push_back(r);
//        std::cout << x << ", " << y << ", " << z << ", " << r << std::endl;
    }


    int n_vl = _velodyneData.size();
    int step = _velodyneData.size()/64;
    tmpdata.clear();
    tmpref.clear();

    std::cout << n_vl << std::endl;
    for(int i=0; i<64;i=i+4) {
        for(int j=0; j<step; j++) {

                tmpdata.push_back(_velodyneData[i*step+j]);
                tmpref.push_back(_velodyneReflectance[i*step+j]);
        }
    }



//    std::cout << std::endl;
}
