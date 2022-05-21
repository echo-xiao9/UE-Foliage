#include "plant_browser.h"
#include "ui_plant_browser.h"

plant_browser::plant_browser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plant_browser)
{
    ui->setupUi(this);
}

plant_browser::~plant_browser()
{
    delete ui;
}

void plant_browser::setPlant(plant* plant){
    ui->hierarchy->setText(QString::number(plant->hierarchy));
    ui->tagList->clear();
    for(auto& tag : plant->tags){
        if(tag.stringTag){
            QListWidgetItem* item = new QListWidgetItem();
            item->setIcon(QIcon(":/icons/string"));
            item->setText(tag.key + ": " + tag.stringValue);
            ui->tagList->addItem(item);
        }
        else{
            QListWidgetItem* item = new QListWidgetItem();
            item->setIcon(QIcon(":/icons/number"));
            item->setText(tag.key + ": " + QString::number(tag.numberValue));
            ui->tagList->addItem(item);
        }
    }
}

