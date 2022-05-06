#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

MainWindow* MainWindow::ptrToMainWindow = nullptr;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , zombiePlant()
    , ui(new Ui::MainWindow)

{
    MainWindow::ptrToMainWindow = this;
    ui->setupUi(this);
    chooseDBRootDir();
    dbOperator = new DBOperator();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseDBRootDir(){
    bool flag = false;
    do{
        DBDir = QFileDialog::getExistingDirectory(this, "选择数据库根路径","",QFileDialog::ShowDirsOnly);
        QFileInfo fileInfo(DBDir);
        flag = fileInfo.isDir();
        if(!flag){
            QMessageBox::critical(this, "路径不存在！", "请重新选择路径");
        }
    }while(!flag);
    do{
        assetDir = QFileDialog::getExistingDirectory(this, "选择资源根路径","",QFileDialog::ShowDirsOnly);
        QFileInfo fileInfo(assetDir);
        flag = fileInfo.isDir();
        if(!flag){
            QMessageBox::critical(this, "路径不存在！", "请重新选择路径");
        }
    }while(!flag);
}

void MainWindow::openDB(){
    QString projName = QInputDialog::getText(this, tr("输入项目名"), tr("请输入项目名："), QLineEdit::Normal,tr(""));
     QFileInfo fileInfo(QFile(DBDir+ "/" + projName + ".db"));
    if(!fileInfo.isFile()){
        QMessageBox::critical(this, "数据库文件不存在！", "请检查您输入的项目名");
        return;
    }
    if(dbOperator->openDB(DBDir+ "/" + projName + ".db") == false){
        QMessageBox::critical(this, "打开失败", "打开数据库失败");
        return;
    }
    currentPlantList = dbOperator->readAllPlantNames();
    updatePlantListDisplay();
    ui->addAsset->setEnabled(true);
    ui->deletAsset->setEnabled(true);
}

void MainWindow::newDB(){
    QString projName = QInputDialog::getText(this, tr("输入项目名"), tr("请输入项目名："), QLineEdit::Normal,tr(""));
     QFileInfo fileInfo(QFile(DBDir+ "/" + projName + ".db"));
    if(fileInfo.isFile()){
        QMessageBox::critical(this, "数据库文件已经存在！", "请检查您输入的项目名");
        return;
    }
    if(dbOperator->newDB(DBDir+ "/" + projName + ".db") == false){
        QMessageBox::critical(this, "创建失败", "创建数据库失败");
        return;
    }
    currentPlantList = dbOperator->readAllPlantNames();
    updatePlantListDisplay();
    ui->addAsset->setEnabled(true);
    ui->deletAsset->setEnabled(true);
}

void MainWindow::MessageBoxForDebug(QString title, QString content){
    QMessageBox::critical(ptrToMainWindow, title, content);
}

void MainWindow::on_newFile_triggered()
{
    newDB();
}


void MainWindow::on_openFile_triggered()
{
    openDB();
}


void MainWindow::on_nameList_currentTextChanged(const QString &currentText)
{
    if(currentText == ""){
        currentPlant = zombiePlant;
        ui->saveName->setEnabled(false);
        ui->addTag->setEnabled(false);
        ui->deleteTag->setEnabled(false);
    }
    else{
        currentPlant = dbOperator->getOnePlantInfo(currentText);
        ui->saveName->setEnabled(true);
        ui->addTag->setEnabled(true);
        ui->deleteTag->setEnabled(true);
    }
    updatePlantDisplay();
}

void MainWindow::updatePlantListDisplay(){
    ui->nameList->clear();
    for(auto& name : currentPlantList){
        ui->nameList->addItem(name);
    }


}

void MainWindow::updatePlantDisplay(){
    ui->tagList->clear();

    ui->NameText->setText(currentPlant.name);
    ui->hierarchyText->setText(QString::number(currentPlant.hierarchy));
    for(auto& tag : currentPlant.tags){
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::UserRole + 0, tag.tagID);
        item->setData(Qt::UserRole + 1, tag.key);
        item->setData(Qt::UserRole + 2, tag.stringTag);
        QString value;
        if(tag.stringTag){
            item->setData(Qt::UserRole + 3, tag.stringValue);
            value = tag.stringValue;
            item->setIcon(QIcon(":/icons/string"));
        }
        else{
            item->setData(Qt::UserRole + 3, tag.numberValue);
            value = QString::number(tag.numberValue);
            item->setIcon(QIcon(":/icons/number"));
        }
        item->setText(tag.key + ":" + value);
        ui->tagList->addItem(item);
        qDebug() << QString("ooringin data is %1").arg(tag.key);

        qDebug() << QString("oringin data is %1").arg(item->data(Qt::UserRole + 1).toString());

    }
}




void MainWindow::on_tagList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current == nullptr){
        ui->KeyText->setText("");
        ui->valueText->setText("");
        ui->saveTags->setEnabled(false);
        ui->resetTags->setEnabled(false);
        return;
    }
    ui->saveTags->setEnabled(true);
    ui->resetTags->setEnabled(true);
    qDebug() << QString("now data is %1").arg(current->data(Qt::UserRole + 1).toString());
    ui->KeyText->setText(current->data(Qt::UserRole + 1).toString());
    if(current->data(Qt::UserRole + 2).toBool()){
        ui->valueText->setText(current->data(Qt::UserRole + 3).toString());

    }
    else{
        ui->valueText->setText(QString::number(current->data(Qt::UserRole + 3).toFloat()));

    }
    qDebug() << QString("id is %1").arg(current->data(Qt::UserRole + 0).toInt());
}


void MainWindow::on_saveTags_clicked()
{
    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }
    if(QMessageBox::Yes != QMessageBox::question(this, "请确认", "确认保存吗")){
        return;
    }
    if(ui->tagList->currentItem() == nullptr)
        return;
    if(ui->tagList->currentItem()->data(Qt::UserRole + 2).toBool()){
        if(ui->tagList->currentItem()->data(Qt::UserRole).toInt() == -1){
            if(dbOperator->addTag(currentPlant.name, ui->KeyText->text(), ui->valueText->text())){
                currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
                updatePlantDisplay();
            }
            else{
                QMessageBox::critical(this, "保存失败！","请仔细检查输入的标签，若无误请与管理员联系");
            }
        }
        else{
            if(dbOperator->saveTag(ui->tagList->currentItem()->data(Qt::UserRole).toInt(), ui->KeyText->text(), ui->valueText->text())){
                currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
                updatePlantDisplay();
            }
            else{
                QMessageBox::critical(this, "保存失败！","请仔细检查输入的标签，若无误请与管理员联系");
            }
        }

    }
    else {
        bool ok = true;
        float value = ui->valueText->text().toFloat(&ok);
        if(!ok){
            QMessageBox::critical(this, "value不是有效数值！","请检查输入的数值");
            return;
        }
        if(ui->tagList->currentItem()->data(Qt::UserRole).toInt() == -1){
            if(dbOperator->addTag(currentPlant.name, ui->KeyText->text(), value)){
                currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
                updatePlantDisplay();
            }
            else{
                QMessageBox::critical(this, "保存失败！","请仔细检查输入的标签，若无误请与管理员联系");
            }
        }
        else{
            if(dbOperator->saveTag(ui->tagList->currentItem()->data(Qt::UserRole).toInt(), ui->KeyText->text(), value)){
                currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
                updatePlantDisplay();
            }
            else{
                QMessageBox::critical(this, "保存失败！","请仔细检查输入的标签，若无误请与管理员联系");
            }
        }

    }
}


void MainWindow::on_addTag_clicked()
{

    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }
    auto chooseDialog = new ChooseNewTagType(this);
    int ret=chooseDialog->exec () ;// 以模态方式显示对话框
    bool isNextTagString = false;
    if (ret==QDialog::Accepted)
    { //OK按钮被按下，获取对话框上的输入，设置行数和列数
        isNextTagString = chooseDialog->stringOrNumberTag();
        delete chooseDialog;
    }
    else{
        delete chooseDialog;
        return;
    }
    plant::tag tag({-1, "New Key", isNextTagString, "New Value", 0, false});
    currentPlant.tags.append(tag);
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(Qt::UserRole + 0, tag.tagID);
    item->setData(Qt::UserRole + 1, tag.key);
    item->setData(Qt::UserRole + 2, tag.stringTag);
    QString value;
    if(tag.stringTag){
        item->setData(Qt::UserRole + 3, tag.stringValue);
        value = tag.stringValue;
        item->setIcon(QIcon(":/icons/string"));
    }
    else{
        item->setData(Qt::UserRole + 3, tag.numberValue);
        value = QString::number(tag.numberValue);
        item->setIcon(QIcon(":/icons/number"));
    }
    item->setText(tag.key + ":" + value);
    ui->tagList->addItem(item);
    ui->tagList->setCurrentItem(item);

}


void MainWindow::on_resetTags_clicked()
{
    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }
    this->on_tagList_currentItemChanged(ui->tagList->currentItem(),ui->tagList->currentItem());
}


void MainWindow::on_deleteTag_clicked()
{
    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }

    if(ui->tagList->currentItem() == nullptr){
        return;
    }
    if(QMessageBox::Yes != QMessageBox::question(this, "请确认", "确认删除吗")){
        return;
    }
    auto current = ui->tagList->currentItem();
    if(current->data(Qt::UserRole + 2).toBool()){
        if(dbOperator->deleteTag(current->data(Qt::UserRole).toInt(),true)){
            currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
            updatePlantDisplay();
            return;
        }
        else{
            QMessageBox::critical(this, "删除失败！","可能是数据库有误，请与管理员联系");
        }
    }
    else{
        if(dbOperator->deleteTag(current->data(Qt::UserRole).toInt(),false)){
            currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
            updatePlantDisplay();
            return;
        }
        else{
            QMessageBox::critical(this, "删除失败！","可能是数据库有误，请与管理员联系");
        }
    }
}


void MainWindow::on_saveName_clicked()
{
    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }
    if(QMessageBox::Yes != QMessageBox::question(this, "请确认", "确认保存吗")){
        return;
    }
    bool ok = false;
    int newHierarchy = ui->hierarchyText->text().toInt(&ok);
    if(ok == false){
        QMessageBox::critical(this, "保存失败！","请检查输入的数值是否合法");
        updatePlantDisplay();
        return;
    }
    if(dbOperator->saveHierarchy(currentPlant.name, newHierarchy)){
        currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
        updatePlantDisplay();
        return;
    }
    else{
        QMessageBox::critical(this, "保存失败！","可能是数据库有误，请与管理员联系");
    }
}


void MainWindow::on_deletAsset_clicked()
{
    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }
    if(QMessageBox::Yes != QMessageBox::question(this, "请确认", QString("确认删除当前植物：%1吗").arg(currentPlant.name))){
        return;
    }
    if(dbOperator->deletePlant(currentPlant.name)){
        currentPlantList = dbOperator->readAllPlantNames();
        updatePlantListDisplay();
    }
    else{
        QMessageBox::critical(this, "删除失败！","可能是数据库有误，请与管理员联系");
    }
}

void MainWindow::on_addAsset_clicked()
{
    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }
    QString newAssetName = QFileDialog::getOpenFileName(this, "选择打开的文件", this->assetDir);
    if(!newAssetName.contains(this->assetDir)){
        QMessageBox::critical(this, "添加失败！","要添加的资源不在当前资源目录中");
        return;
    }
    newAssetName = newAssetName.mid(assetDir.size(), -1);
    if(dbOperator->addPlant(newAssetName)){
        currentPlantList = dbOperator->readAllPlantNames();
        updatePlantListDisplay();
    }
    else{
        QMessageBox::critical(this, "添加失败！","可能是数据库中已经包含该资源");
    }
}


void MainWindow::on_setDBDir_triggered()
{
    bool flag;
    QString tmpDBDir;
    do{
        tmpDBDir = QFileDialog::getExistingDirectory(this, "选择数据库根路径","",QFileDialog::ShowDirsOnly);
        if(tmpDBDir == "")
            return;
        QFileInfo fileInfo(tmpDBDir);
        flag = fileInfo.isDir();
        if(!flag){
            QMessageBox::critical(this, "路径不存在！", "请重新选择路径");
        }
    }while(!flag);
    DBDir = tmpDBDir;
}


void MainWindow::on_setAssetDir_triggered()
{
    bool flag;
    QString tmpAssetDir;
    do{
        tmpAssetDir = QFileDialog::getExistingDirectory(this, "选择资源根路径","",QFileDialog::ShowDirsOnly);
        if(tmpAssetDir == "")
            return;
        QFileInfo fileInfo(tmpAssetDir);
        flag = fileInfo.isDir();
        if(!flag){
            QMessageBox::critical(this, "路径不存在！", "请重新选择路径");
        }
    }while(!flag);
    assetDir = tmpAssetDir;
}

