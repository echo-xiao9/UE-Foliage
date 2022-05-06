#include "choosenewtagtype.h"
#include "ui_choosenewtagtype.h"
#include "mainwindow.h"
ChooseNewTagType::ChooseNewTagType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseNewTagType)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);

}

ChooseNewTagType::~ChooseNewTagType()
{
    delete ui;
}

void ChooseNewTagType::on_buttonBox_accepted()
{

}


void ChooseNewTagType::on_buttonBox_rejected()
{

}

bool ChooseNewTagType::stringOrNumberTag(){
    return ui->stringRadio->isChecked();
}

