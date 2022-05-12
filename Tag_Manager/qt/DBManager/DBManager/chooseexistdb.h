#ifndef CHOOSEEXISTDB_H
#define CHOOSEEXISTDB_H

#include <QDialog>

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
