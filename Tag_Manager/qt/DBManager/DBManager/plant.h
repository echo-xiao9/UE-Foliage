#ifndef PLANT_H
#define PLANT_H
#include<QString>
#include<QVector>
#include <QPair>
class plant
{
public:
    plant();
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
};

#endif // PLANT_H
