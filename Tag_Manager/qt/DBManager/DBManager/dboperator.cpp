#include "dboperator.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
DBOperator::DBOperator()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}
bool DBOperator::isOpen(){
    return open;
}
bool DBOperator::openDB(QString file){
    if(open){
        db.close();
        open = false;
    }

    db.setDatabaseName(file);

    if (!db.open())
        {
            qDebug() << "Error: Failed to connect database." << db.lastError();
            return false;
        }
        else
        {
            qDebug() << "Succeed to connect database." ;
            open = true;
            QSqlQuery query(db);
            if(!query.exec("PRAGMA foreign_keys = ON"))//使外键功能生效
                 {
                     qDebug()<<"No Effect!";
                 }
        }
    return true;
}

bool DBOperator::newDB(QString file){
    if(openDB(file) == false){
        return false;
    }
    if(excute_sql_file(":/sqlScripts/formwork.sql") != 0){
        return false;
    }
    return true;


}

QString DBOperator::getAssetDir(){
    if(!open){
        qDebug() << "not open";
        return "";
    }
    QString select_all_sql = "select dir from projectDir";
    QSqlQuery sql_query;
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString name = sql_query.value(0).toString();
            qDebug()<<QString("name: %1").arg(name);
            return name;
        }
    }
    return "";
}

bool DBOperator::setAssetDir(QString dir){
    QString update_sql = "insert or replace into projectdir (id, dir) values (1,:dir)";
    QSqlQuery sql_query;
    sql_query.prepare(update_sql);
    sql_query.bindValue(":dir", dir);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "updated!";
        return true;
    }
}

int DBOperator::excute_sql_file(QString path)
{
    std::string stdStrPath = path.toStdString();
    char * sql_file_path = stdStrPath.data();
    int iRet = 0;
    QFile qfile(sql_file_path);
    if (!qfile.exists()) {
        return -1;
    }
    if (!qfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return -1;
    }
    QTextStream in(&qfile);
    QString qstr_file_data = in.readAll();
    QSqlQuery qsql(db);
    QStringList qstrlist_sql =  qstr_file_data.split(";");
    for (int i = 0; i < qstrlist_sql.size() - 1; i++) {
        QString qstr_sql_part = qstrlist_sql.at(i).toUtf8();
        qDebug() << qstr_sql_part;
        bool sql_result = qsql.exec(qstr_sql_part);
        if (!sql_result) {
            QSqlError sql_error = qsql.lastError();
            qDebug() << sql_error.text() ;
            iRet = -1;
            break;
        }
    }
    return iRet;
}

QVector<QString> DBOperator::readAllPlantNames(){
    QVector<QString> result;
    if(!open){
        return result;
    }
    QString select_all_sql = "select name from plant";
    QSqlQuery sql_query;
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString name = sql_query.value(0).toString();
            qDebug()<<QString("name: %1").arg(name);
            result.append(name);
        }
    }
    return result;
}

QVector<QString> DBOperator::searchPlantNames(QString search){
    QVector<QString> result;
    if(!open){
        return result;
    }
    QString select_all_sql = "select name from plant where name like :name";
    QSqlQuery sql_query;
    sql_query.prepare(select_all_sql);
    sql_query.bindValue(":name", QString("%%1%").arg(search));
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString name = sql_query.value(0).toString();
            qDebug()<<QString("name: %1").arg(name);
            result.append(name);
        }
    }
    return result;
}

plant DBOperator::getOnePlantInfo(QString plantName){
    plant result;
    if(!open){
        return result;
    }
    result.name = plantName;
    {
        QString select_sql = "select distinct plant.id, plant.hierarchy, image.imageBin, image.imageformat from plant, image where plant.name = :name and image.plantid = plant.id";
        QSqlQuery sql_query;
        sql_query.prepare(select_sql);
        sql_query.bindValue(":name",plantName);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                int id = sql_query.value(0).toInt();
                int hierarchy = sql_query.value(1).toInt();
                QByteArray img = sql_query.value(2).toByteArray();
                QString fmt = sql_query.value(3).toString();
                qDebug()<<QString("name: %1").arg(hierarchy);
                qDebug() << img.size();
                result.plantID = id;
                result.hierarchy = hierarchy;
                result.image = img;
                result.imageFmt = fmt;
            }
        }
    }
    {
        QString select_sql = "select distinct numberTag.key, numberTag.value, numberTag.id from plant, numberTag where plant.name = :name and plant.id = numberTag.plantid";
        QSqlQuery sql_query;
        sql_query.prepare(select_sql);
        sql_query.bindValue(":name",plantName);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                QString key = sql_query.value(0).toString();
                float value = sql_query.value(1).toFloat();
                int id = sql_query.value(2).toInt();
                qDebug()<<QString("key: %1, value: %2").arg(key).arg(value);
                result.tags.append({id, key,false, "", value, true});
            }
        }
    }
    {
        QString select_sql = "select distinct stringTag.key, stringTag.value, stringTag.id from plant, stringTag where plant.name = :name and plant.id = stringTag.plantid";
        QSqlQuery sql_query;
        sql_query.prepare(select_sql);
        sql_query.bindValue(":name",plantName);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                QString key = sql_query.value(0).toString();
                QString value = sql_query.value(1).toString();
                int id = sql_query.value(2).toInt();
                qDebug()<<QString("key: %1, value: %2").arg(key).arg(value);
                result.tags.append({id, key,true, value, 0, true});
            }
        }
    }



    return result;
}

bool DBOperator::saveTag(int tagID, QString key, QString value){
    QString update_sql = "update stringTag set key = :key, value = :value where id = :id";
    QSqlQuery sql_query;
    sql_query.prepare(update_sql);
    sql_query.bindValue(":key", key);
    sql_query.bindValue(":value",value);
    sql_query.bindValue(":id", tagID);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "updated!";
        return true;
    }
}

bool DBOperator::saveTag(int tagID, QString key, float value){
    QString update_sql = "update numberTag set key = :key, value = :value where id = :id";
    QSqlQuery sql_query;
    sql_query.prepare(update_sql);
    sql_query.bindValue(":key", key);
    sql_query.bindValue(":value",value);
    sql_query.bindValue(":id", tagID);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "updated!";
        return true;
    }
}

bool DBOperator::addTag(QString plantName, QString key, float value){
    QString insert_sql = "insert into numberTag (key, value, plantID) values(:key, :value, (select id from plant where name = :name))";
    QSqlQuery sql_query;
    sql_query.prepare(insert_sql);
    sql_query.bindValue(":key",key);
    sql_query.bindValue(":value",value);
    sql_query.bindValue(":name", plantName);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else{
        qDebug() << "Inserted!";
        return true;
    }
}

bool DBOperator::addTag(QString plantName, QString key, QString value){
    QString insert_sql = "insert into stringTag (key, value, plantID) values(:key, :value, (select id from plant where name = :name))";
    QSqlQuery sql_query;
    sql_query.prepare(insert_sql);
    sql_query.bindValue(":key",key);
    sql_query.bindValue(":value",value);
    sql_query.bindValue(":name", plantName);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else{
        qDebug() << "Inserted!";
        return true;
    }
}

bool DBOperator::deleteTag(int tagID, bool isStringTag){
    QString delete_sql;
    if(isStringTag)delete_sql = "delete from stringTag where id = :id";
    else delete_sql = "delete from numberTag where id = :id";
    QSqlQuery sql_query;
    sql_query.prepare(delete_sql);
    sql_query.bindValue(":id",tagID);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else{
        qDebug() << "Deleted!";
        return true;
    }
}

bool DBOperator::saveHierarchy(QString plantName, int hierarchy){
    QString update_sql = "update plant set hierarchy = :hierarchy where name = :name";
    QSqlQuery sql_query;
    sql_query.prepare(update_sql);
    sql_query.bindValue(":hierarchy",hierarchy);
    qDebug() << QString("hierarchy is %1").arg(hierarchy);
    sql_query.bindValue(":name",plantName);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "updated!";
        return true;
    }
}

bool DBOperator::saveImage(QString plantName, QByteArray img, QString fmt){
    QString update_Sql = "insert or replace into image (imagebin, imageformat, plantid) values (:img, :fmt, (select id from plant where name = :name))";
    QSqlQuery sql_query;
    sql_query.prepare(update_Sql);
    sql_query.bindValue(":img",img);
    sql_query.bindValue(":fmt",fmt);
    sql_query.bindValue(":name",plantName);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "updated!";
        return true;
    }

}

bool DBOperator::deletePlant(QString plantName){
    QString delete_sql = "delete from plant where name = :name";
    QSqlQuery sql_query;
    sql_query.prepare(delete_sql);
    sql_query.bindValue(":name", plantName);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Deleted!";
        return true;
    }
}

bool DBOperator::addPlant(QString plantName, plant* formwork){
    {
        QString add_sql = "insert into plant (name, hierarchy) values (:name, 0)";
        QSqlQuery sql_query;
        sql_query.prepare(add_sql);
        sql_query.bindValue(":name",plantName);
        if(!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
            return false;
        }
        else
        {
            qDebug() << "Inserted!";
        }
    }
    {
        QString add_sql = "insert into image (plantid) values ((select id from plant where name = :name))";
        QSqlQuery sql_query;
        sql_query.prepare(add_sql);
        sql_query.bindValue(":name",plantName);
        if(!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
            return false;
        }
        else
        {
            qDebug() << "Image Inserted!";
        }
    }
    if(formwork != nullptr){
        qDebug() << "applying formwork";
        saveHierarchy(plantName, formwork->hierarchy);
        for(auto& tag : formwork->tags){
            qDebug() << "model key is: ";
            qDebug() << tag.key;
            if(tag.stringTag){
                addTag(plantName, tag.key, tag.stringValue);
            }
            else{
                addTag(plantName, tag.key, tag.numberValue);
            }
        }
    }
    return true;
}

void DBOperator::tryToSaveImage(QString plantName, QString imgFilename){
    QString find_sql = "select id from plant where name like :name";
    QSqlQuery sql_query;
    sql_query.prepare(find_sql);
    sql_query.bindValue(":name", QString("%1%").arg(plantName + "."));
    bool found = false;
    int id;
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return;
    }
    else
    {
        while(sql_query.next())
        {
            id = sql_query.value(0).toInt();
            qDebug()<<QString("find plant id is : %1").arg(id);
            found = true;
        }
    }
    if(found){
        QFileInfo fileInfo(imgFilename);
        if(!fileInfo.isFile()){
            return;
        }
        QString fmt = fileInfo.suffix();
        QFile imgFile(imgFilename);
        if (!imgFile.open(QIODevice::ReadOnly | QIODevice::Truncate))
            return;
        auto imgByteArray = imgFile.readAll();
         QString update_sql = "insert or replace into image (imagebin, imageformat, plantid) values (:img, :fmt, :id)";
         QSqlQuery sql_query;
         sql_query.prepare(update_sql);
         sql_query.bindValue(":img",imgByteArray);
         sql_query.bindValue(":fmt", fmt);
         sql_query.bindValue(":id",id);
         if(!sql_query.exec())
         {
             qDebug() << sql_query.lastError();
             return;
         }
         else
         {
             qDebug() << "Image Inserted!";
             qDebug() << imgFilename;
         }
    }
}

