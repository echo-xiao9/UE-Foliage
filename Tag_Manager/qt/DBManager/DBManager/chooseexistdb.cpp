#include "chooseexistdb.h"
#include "ui_chooseexistdb.h"
ChooseExistDB::ChooseExistDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseExistDB)
{
    ui->setupUi(this);
}

ChooseExistDB::~ChooseExistDB()
{
    delete ui;
}

void ChooseExistDB::setFilenames(QVector<QString> filenames){
    for(auto name: filenames){
        ui->filenameList->addItem(name);
    }
}

QString ChooseExistDB::getProjName(){
    return chosen;
}

void ChooseExistDB::on_filenameList_currentTextChanged(const QString &currentText)
{
    chosen = currentText;
}

