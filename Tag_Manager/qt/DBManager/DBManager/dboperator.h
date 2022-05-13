#ifndef DBOPERATOR_H
#define DBOPERATOR_H
#include <QString>
#include <QSqlDatabase>
#include <QVector>
#include "plant.h"

class DBOperator
{
private:
    bool open = false;
    QSqlDatabase db;
    int excute_sql_file(QString file);
public:
    DBOperator();
    bool openDB(QString file);
    bool newDB(QString file);
    void closeDB();
    bool isOpen();
    QVector<QString> readAllPlantNames();
    QVector<QString> searchPlantNames(QString search);
    plant getOnePlantInfo(QString plantName);
    bool saveTag(int tagID, QString key, QString value);
    bool saveTag(int tagID, QString key, float value);
    bool addTag(QString plantName, QString key, QString value);
    bool addTag(QString plantName, QString key, float value);
    bool deleteTag(int tagID, bool isStringTag);
    bool saveHierarchy(QString plantName, int hierarchy);
    bool saveImage(QString plantName, QByteArray img, QString fmt);
    bool deletePlant(QString plantName);
    bool addPlant(QString plantName);
    QString getAssetDir();
    bool setAssetDir(QString dir);
};

#endif // DBOPERATOR_H
