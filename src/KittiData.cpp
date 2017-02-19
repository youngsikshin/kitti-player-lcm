#include <iostream>

#include <QDir>
#include <QTextStream>
#include <QDataStream>

#include <QDebug>
#include "KittiData.h"

KittiData::KittiData()
{

}

KittiData::KittiData(QString path, QString gtFname)
{
    _leftImgPath = path+"image_0";
    _rightImgPath = path+"image_1";
    _velodynePath = path+"velodyne";
    _gtFname = gtFname;

    _flistLeftImg = get_filelist(_leftImgPath, "*.png");
    _flistRightImg = get_filelist(_rightImgPath,"*.png");
    _flistVelodyne = get_filelist(_velodynePath,"*.bin");

    velodyneLayer = Layer64;

    // Read times.txt
    QFile _ftimes(path+"times.txt");
    if (!_ftimes.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream timesIn(&_ftimes);
    while (!timesIn.atEnd()) {
        QString line = timesIn.readLine();
        _times.push_back(line.toDouble());
    }

    // Read GT poses
    QFile _gtPoses(_gtFname);
    if (!_gtPoses.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream posesIn(&_gtPoses);
    while (!posesIn.atEnd()) {
        QString line = posesIn.readLine();
        QStringList list = line.split(" ");
        QMatrix4x4 Tgt(list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), list[3].toDouble(),
                       list[4].toDouble(), list[5].toDouble(), list[6].toDouble(), list[7].toDouble(),
                       list[8].toDouble(), list[9].toDouble(), list[10].toDouble(), list[11].toDouble(),
                       0,    0,    0,    1);
        _poses.push_back(Tgt);

//        qDebug() << Tgt << endl;
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

    bool init_azimuth = false;
    double prev_azimuth;
    QVector<GLfloat> singleLayerData;
    QVector<GLfloat> singleLayerReflectance;

    while(!in.atEnd()) {
        in.setByteOrder(QDataStream::LittleEndian);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        double x, y, z, r;

        in >> x >> y >> z >> r;

        if (velodyneLayer == Layer16) {
            double azimuth = atan2(static_cast<double> (y), static_cast<double>(x));
            azimuth = (azimuth > 0 ? azimuth : (2*M_PI + azimuth));

            if (!init_azimuth) {
                prev_azimuth = azimuth;
                init_azimuth = true;
            }
            else {
                if((azimuth - prev_azimuth) < -0.2) {
                    if(cnt%4 == 0) {
                        insert_QVector(_velodyneData, singleLayerData);
                        insert_QVector(_velodyneReflectance, singleLayerReflectance);
                    }
                    cnt++;
                    singleLayerData.clear();
                    singleLayerReflectance.clear();
                }
                prev_azimuth = azimuth;
            }

            singleLayerData.push_back(x);
            singleLayerData.push_back(y);
            singleLayerData.push_back(z);
            singleLayerReflectance.push_back(r);
            singleLayerReflectance.push_back(r);
            singleLayerReflectance.push_back(r);
        }
        else if (velodyneLayer == Layer64){
            _velodyneData.push_back(x);
            _velodyneData.push_back(y);
            _velodyneData.push_back(z);
            _velodyneReflectance.push_back(r);
            _velodyneReflectance.push_back(r);
            _velodyneReflectance.push_back(r);
        }

    }

    if (velodyneLayer == Layer16 && cnt%4 == 0) {
        insert_QVector(_velodyneData, singleLayerData);
        insert_QVector(_velodyneReflectance, singleLayerReflectance);
    }

    singleLayerData.clear();
    singleLayerReflectance.clear();
}

void KittiData::insert_QVector(QVector<GLfloat>& dst, const QVector<GLfloat>& src)
{
    for(int i=0; i<src.size(); i++) {
        dst.push_back(src[i]);;
    }
}
