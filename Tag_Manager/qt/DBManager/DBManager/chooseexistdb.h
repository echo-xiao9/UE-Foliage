#ifndef CHOOSEEXISTDB_H
#define CHOOSEEXISTDB_H

#include <QDialog>
/*这个文件用来定义一个窗口，在选取数据库的时候能够显示列表*/
namespace Ui {
class ChooseExistDB;
}

class ChooseExistDB : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseExistDB(QWidget *parent = nullptr);
    ~ChooseExistDB();
    void setFilenames(QVector<QString> filenames);
    QString getProjName();

private slots:
    void on_filenameList_currentTextChanged(const QString &currentText);

private:
    Ui::ChooseExistDB *ui;
    QString chosen;
};

#endif // CHOOSEEXISTDB_H
