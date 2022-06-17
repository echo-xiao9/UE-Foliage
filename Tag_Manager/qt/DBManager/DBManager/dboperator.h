#ifndef DBOPERATOR_H
#define DBOPERATOR_H
#include <QString>
#include <QSqlDatabase>
#include <QVector>
#include "plant.h"
/*这个文件定义了与数据库文件交互的接口*/

class DBOperator
{
private:
    bool open = false;
    QSqlDatabase db;
    int excute_sql_file(QString file);
public:
    DBOperator();

    //打开一个数据库
    bool openDB(QString file);

    //创建并且打开一个数据库
    bool newDB(QString file);

    //关闭数据库
    void closeDB();

    //判断是否打开，只是单纯地返回open这个变量
    bool isOpen();

    //获取数据库中所有植物资源的名字，用于显示全部植物列表
    QVector<QString> readAllPlantNames();

    //获取数据库中带搜索植物的列表，用于显示搜索的植物列表
    QVector<QString> searchPlantNames(QString search);

    //获取一个植物的相关信息，包括它的缩略图和所有TAG信息
    plant getOnePlantInfo(QString plantName);

    //保存一个TAG，TAG的类型是String
    bool saveTag(int tagID, QString key, QString value);

    //保存一个TAG，TAG的类型是浮点数
    bool saveTag(int tagID, QString key, float value);

    //新增一个TAG，TAG的类型是String
    bool addTag(QString plantName, QString key, QString value);

    //新增一个TAG，TAG的类型是数值型
    bool addTag(QString plantName, QString key, float value);

    //删除一个TAG，值得注意的是数据库中TAG的ID对于该系统是可见的，作为唯一标识
    bool deleteTag(int tagID, bool isStringTag);

    //修改植物层级
    bool saveHierarchy(QString plantName, int hierarchy);

    //保存缩略图
    bool saveImage(QString plantName, QByteArray img, QString fmt);

    //保存缩略图
    void tryToSaveImage(QString plantName, QString imgFileName);

    //删除一个植物资源
    bool deletePlant(QString plantName);

    //新增一个植物
    bool addPlant(QString plantName, plant* formwork = nullptr);

    //获取数据库中存储的项目资源根路径
    QString getAssetDir();

    //修改数据库中存储的项目资源根路径
    bool setAssetDir(QString dir);

};

#endif // DBOPERATOR_H
