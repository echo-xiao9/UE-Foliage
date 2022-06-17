#ifndef PLANT_H
#define PLANT_H
#include<QString>
#include<QVector>
#include <QPair>
//定义了一个植物类，用于存储相关信息，并且可以导出到文件
class plant
{
public:
    plant();
    plant(QString filename);
    int plantID;
    QString name = "null";
    int hierarchy = 0;
    struct tag{
        int tagID;
        QString key;
        bool stringTag;
        QString stringValue;
        float numberValue;
        bool inDatabase;
    };
    QVector<tag> tags;
    QByteArray image;
    QString imageFmt;
    bool exportPlant(QString file);
};

#endif // PLANT_H
