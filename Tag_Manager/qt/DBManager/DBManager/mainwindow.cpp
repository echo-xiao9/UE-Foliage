#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QBuffer>
#include <QTimer>
#include <QStack>
#include <QPair>
#include <QVector>
#include <QCursor>
#include "plant_browser.h"

MainWindow* MainWindow::ptrToMainWindow = nullptr;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , zombiePlant()
    , ui(new Ui::MainWindow)

{
    MainWindow::ptrToMainWindow = this;
    ui->setupUi(this);
    setRootDir();
    dbOperator = new DBOperator();
    ui->imgLabel->installEventFilter(this);
    ui->progressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRootDir(){
    auto dir = readDBBaseDir();
    if(dir == ""){
        chooseDBRootDir();
    }
    else{
        DBDir = dir;
    }
}


QString MainWindow::readDBBaseDir(){
    QFile   aFile("./DBDir.txt");
    if (!aFile.exists()) //文件不存在
        return "";
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    QString dir = QString(aFile.readAll());
    aFile.close();
    QFileInfo fileInfo(dir);
    if(!fileInfo.isDir()){
        return "";
    }
    return dir;
}

void MainWindow::writeDBBaseDir(QString dir){
    QFile aFile("./DBDir.txt");
    if(aFile.exists()){
        aFile.resize(0);
    }
    aFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QByteArray  strBytes=dir.toUtf8();
    aFile.write(strBytes,strBytes.length());  //写入文件
    aFile.close();
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
    writeDBBaseDir(DBDir);

}

void MainWindow::openDB(){
    QDir dir(DBDir);
    QStringList nameFilters;
    nameFilters << "*.db";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    QVector<QString> projNames;
    for(auto file: files){
        projNames.append(file.mid(0, file.size() - 3));
    }


    auto chooseDialog = new ChooseExistDB(this);
    chooseDialog->setFilenames(projNames);
    int ret=chooseDialog->exec () ;// 以模态方式显示对话框
    QString projName = "";
    if (ret==QDialog::Accepted)
    { //OK按钮被按下，获取对话框上的输入，设置行数和列数
        projName = chooseDialog->getProjName();
        delete chooseDialog;
    }
    else{
        delete chooseDialog;
        return;
    }
//    projName = QInputDialog::getText(this, tr("输入项目名"), tr("请输入项目名："), QLineEdit::Normal,tr(""));
    if(projName == ""){
        return;
    }
     QFileInfo fileInfo(QFile(DBDir+ "/" + projName + ".db"));
    if(!fileInfo.isFile()){
        QMessageBox::critical(this, "数据库文件不存在！", "请检查您输入的项目名");
        return;
    }
    if(dbOperator->openDB(DBDir+ "/" + projName + ".db") == false){
        QMessageBox::critical(this, "打开失败", "打开数据库失败");
        return;
    }
    assetDir = dbOperator->getAssetDir();
    qDebug() << assetDir;
    currentPlantList = dbOperator->readAllPlantNames();
    updatePlantListDisplay();
    ui->addAsset->setEnabled(true);
    ui->deletAsset->setEnabled(true);
    ui->searchPlant->setEnabled(true);
    ui->searchName->setEnabled(true);
    ui->resetSearch->setEnabled(true);
    ui->setAssetDir->setEnabled(true);
    ui->importPic->setEnabled(true);
}

void MainWindow::newDB(){

    QString projName = QInputDialog::getText(this, tr("输入项目名"), tr("请输入项目名："), QLineEdit::Normal,tr(""));
    if(projName.isEmpty())return;
     QFileInfo fileInfo(QFile(DBDir+ "/" + projName + ".db"));
    if(fileInfo.isFile()){
        QMessageBox::critical(this, "数据库文件已经存在！", "请检查您输入的项目名");
        return;
    }
    QString newAssetDir = setAssetDir();
    if(newAssetDir == ""){
        return;
    }
    if(dbOperator->newDB(DBDir+ "/" + projName + ".db") == false){
        QMessageBox::critical(this, "创建失败", "创建数据库失败");
        return;
    }
    assetDir = newAssetDir;
    dbOperator->setAssetDir(assetDir);
    currentPlantList = dbOperator->readAllPlantNames();
    updatePlantListDisplay();
    ui->addAsset->setEnabled(true);
    ui->deletAsset->setEnabled(true);
    ui->searchPlant->setEnabled(true);
    ui->searchName->setEnabled(true);
    ui->resetSearch->setEnabled(true);
    ui->setAssetDir->setEnabled(true);
    ui->importPic->setEnabled(true);
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
        ui->NameText->setEnabled(false);
        ui->hierarchyText->setEnabled(false);
        ui->exportPlant->setEnabled(false);
        ui->addTag->setEnabled(false);
        ui->deleteTag->setEnabled(false);
        this->imgLabelEnabled = false;
        ui->chooseColor->setEnabled(false);
    }
    else{
        currentPlant = dbOperator->getOnePlantInfo(currentText);
        ui->saveName->setEnabled(true);
        ui->NameText->setEnabled(true);
        ui->hierarchyText->setEnabled(true);
        ui->exportPlant->setEnabled(true);
        ui->addTag->setEnabled(true);
        ui->deleteTag->setEnabled(true);
        this->imgLabelEnabled = true;
        ui->chooseColor->setEnabled(true);
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
    QPixmap pixmap;
    if(currentPlant.image.size() == 0 || pixmap.loadFromData(currentPlant.image,currentPlant.imageFmt.toStdString().c_str()) == false){
        qDebug()<< "no img available";
        ui->imgLabel->setPixmap(QPixmap(":/Image/blackScreen.png"));
    }
    else{
        pixmap = pixmap.scaled(300, 300,  Qt::KeepAspectRatio);
        ui->imgLabel->setPixmap(pixmap);
    }
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
        ui->KeyText->setEnabled(false);
        ui->valueText->setEnabled(false);
        ui->resetTags->setEnabled(false);
        return;
    }
    ui->saveTags->setEnabled(true);
    ui->KeyText->setEnabled(true);
    ui->valueText->setEnabled(true);
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
    tagChanged = false;
}


void MainWindow::on_saveTags_clicked()
{
    if(!tagChanged){
        int currentRow = ui->tagList->currentRow();
        if(currentRow >= 0 && currentRow < ui->tagList->count() - 1){
            ui->tagList->setCurrentRow(currentRow + 1);
            ui->KeyText->setFocus();
            QTimer::singleShot(0, ui->KeyText, &QLineEdit::selectAll);
        }
        else{
            while(true){
                if(ui->nameList->currentRow() >= 0 && ui->nameList->currentRow() < ui->nameList->count() - 1 ){
                    ui->nameList->setCurrentRow(ui->nameList->currentRow() + 1);
                    if(ui->tagList->count() != 0){
                        ui->tagList->setCurrentRow(0);
                        ui->KeyText->setFocus();
                        QTimer::singleShot(0, ui->KeyText, &QLineEdit::selectAll);
                        break;
                    }
                }
                else{
                    break;
                }
            }
        }
        return;
    }
    if(!dbOperator || !dbOperator->isOpen()){
        QMessageBox::critical(this, "错误","请先打开数据库！");
        return;
    }
    if(QMessageBox::Yes != QMessageBox::question(this, "请确认", "确认保存吗")){
        return;
    }
    if(ui->tagList->currentItem() == nullptr)
        return;
    int currentRow = ui->tagList->currentRow();
    qDebug() << "current tag id is";
    qDebug() << currentRow;
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
    qDebug() << "current tag id is (down)";
    qDebug() << currentRow;
    if(currentRow >= 0 && currentRow < ui->tagList->count() - 1){
        ui->tagList->setCurrentRow(currentRow + 1);
        ui->KeyText->setFocus();
        QTimer::singleShot(0, ui->KeyText, &QLineEdit::selectAll);
    }
    else{
        while(true){
            if(ui->nameList->currentRow() >= 0 && ui->nameList->currentRow() < ui->nameList->count() - 1 ){
                ui->nameList->setCurrentRow(ui->nameList->currentRow() + 1);
                if(ui->tagList->count() != 0){
                    ui->tagList->setCurrentRow(0);
                    ui->KeyText->setFocus();
                    QTimer::singleShot(0, ui->KeyText, &QLineEdit::selectAll);
                    break;
                }
            }
            else{
                break;
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
    tagChanged = true;

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
    if(currentPlant.name == "__default__")return;
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
    auto newAssetNames = QFileDialog::getOpenFileNames(this, "选择打开的文件", this->assetDir);
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    int i = 0;
    int success = 0;
    for(auto newAssetName : newAssetNames){
        if(newAssetName == "")continue;
        if(!newAssetName.contains(this->assetDir)){
            QMessageBox::critical(this, "添加失败！","要添加的资源不在当前资源目录中：" + newAssetName);
            continue;
        }
        newAssetName = newAssetName.mid(assetDir.size(), -1);
        qDebug() << "is formwork null?";
        qDebug() << (this->formworkPlant == nullptr);
        if(dbOperator->addPlant(newAssetName, this->formworkPlant)){
            success ++;
        }
        else{
            QMessageBox::critical(this, "添加失败！","可能是数据库中已经包含该资源：" + newAssetName);
        }
        ui->progressBar->setValue(((float)(++ i)) / newAssetNames.size() * 100);
        QApplication::processEvents();
    }
    currentPlantList = dbOperator->readAllPlantNames();
    updatePlantListDisplay();
    QMessageBox::information(this, "添加完成", "添加完成！已添加" + QString::number(success) + "of" + QString::number(newAssetNames.size()));
    ui->progressBar->setVisible(false);
}


void MainWindow::on_setDBDir_triggered()
{
    bool flag;
    QString tmpDBDir;
    do{
        tmpDBDir = QFileDialog::getExistingDirectory(this, "选择数据库根路径",DBDir,QFileDialog::ShowDirsOnly);
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
    QString originDir = dbOperator->getAssetDir();
    do{
        tmpAssetDir = QFileDialog::getExistingDirectory(this, "选择资源根路径",originDir,QFileDialog::ShowDirsOnly);
        if(tmpAssetDir == "")
            return;
        QFileInfo fileInfo(tmpAssetDir);
        flag = fileInfo.isDir();
        if(!flag){
            QMessageBox::critical(this, "路径不存在！", "请重新选择路径");
        }
    }while(!flag);
    assetDir = tmpAssetDir;
    dbOperator->setAssetDir(assetDir);
}

QString MainWindow::setAssetDir(){
    bool flag;
    QString tmpAssetDir;
    QString originDir = dbOperator->getAssetDir();
    do{
        tmpAssetDir = QFileDialog::getExistingDirectory(this, "选择资源根路径",originDir,QFileDialog::ShowDirsOnly);
        if(tmpAssetDir == ""){
            return "";
        }
        QFileInfo fileInfo(tmpAssetDir);
        flag = fileInfo.isDir();
        if(!flag){
            QMessageBox::critical(this, "路径不存在！", "请重新选择路径");
        }
    }while(!flag);
    return tmpAssetDir;
}


void MainWindow::on_searchPlant_clicked()
{
    currentPlantList = dbOperator->searchPlantNames(ui->searchName->text());
    updatePlantListDisplay();
}


void MainWindow::on_resetSearch_clicked()
{
    ui->searchName->setText("");
    currentPlantList = dbOperator->readAllPlantNames();
    updatePlantListDisplay();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if(obj == ui->imgLabel)
    {
         if (event->type() == QEvent::MouseButtonPress && ui->chooseColor->isChecked() == false)
         {
            if(this->imgLabelEnabled){
                QString curPath=assetDir;//获取系统当前目录
                QString dlgTitle="选择一张图片"; //对话框标题
                QString imgFilename=QFileDialog::getOpenFileName(this,dlgTitle,curPath);
                QFileInfo fileInfo(imgFilename);
                if(!fileInfo.isFile()){
                    return true;
                }
                QString fmt = fileInfo.suffix();
                QFile imgFile(imgFilename);
                if (!imgFile.open(QIODevice::ReadOnly | QIODevice::Truncate))
                    return true;
                auto imgByteArray = imgFile.readAll();
                if(!(dbOperator->saveImage(currentPlant.name, imgByteArray,fmt))){
                    QMessageBox::critical(this, "保存失败！", "请检查您的图片文件");
                }
                else{
                    currentPlant = dbOperator->getOnePlantInfo(currentPlant.name);
                    updatePlantDisplay();
                }

            }
            return true;
         }
         else if(event->type() == QEvent::MouseButtonPress && ui->chooseColor->isChecked() == true && !(ui->imgLabel->pixmap().isNull())){
              QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
              QPointF p = mouseEvent->pos();
              auto color = ui->imgLabel->pixmap().toImage().pixel(p.x(), p.y());
              int r;
              int g;
              int b;
              QColor(color).getRgb(&r, &g, &b);
              ui->RGB_text->setText(QString("R: %1, G: %2, B: %3").arg(r).arg(g).arg(b));
              int h;
              int s;
              int v;
              QColor(color).getHsv(&h, &s, &v);
              ui->HSV_text->setText(QString("H: %1, S: %2, V: %3").arg(QString::number(float(h),'f',1)).arg(QString::number(s / 255.0 * 100.0,'f',1)).arg(QString::number(v / 255.0 * 100.0,'f',1)));
         }
    }
    return false;
}

void MainWindow::on_exportPlant_clicked()
{
    QString plantFilename=QFileDialog::getSaveFileName(this,"保存植物模板","","Plant model files(*.mod)");
    if(plantFilename != ""){
        currentPlant.exportPlant(plantFilename);
    }

}


void MainWindow::on_resetPlantFormwork_2_triggered()
{
    if(this->formworkPlant != nullptr){
        delete (this->formworkPlant);
    }
    this->formworkPlant = nullptr;
    QMessageBox::information(this, "重置成功", "植物模板已被重置");
}


void MainWindow::on_setPlantFormwork_triggered()
{
    QString plantFilename = QFileDialog::getOpenFileName(this,"选择植物模板","","Plant model files(*.mod)");
    if(plantFilename == "")return;
    if(this->formworkPlant != nullptr){
        delete (this->formworkPlant);
    }
    this->formworkPlant = new plant(plantFilename);
}


void MainWindow::on_checkPlantFormwork_triggered()
{
    if(this->formworkPlant == nullptr){
        QMessageBox::information(this, "没有模板", "当前没有设置植物模板");
        return;
    }
    auto infoDialog = new plant_browser(this);
    infoDialog->setPlant(formworkPlant);
    infoDialog->exec () ;// 以模态方式显示对话框
}


void MainWindow::on_KeyText_returnPressed()
{
    if(ui->saveTags->isEnabled())
        on_saveTags_clicked();
}


void MainWindow::on_valueText_returnPressed()
{
    if(ui->saveTags->isEnabled())
        on_saveTags_clicked();
}


void MainWindow::on_KeyText_textChanged(const QString &arg1)
{
    tagChanged = true;
}


void MainWindow::on_valueText_textChanged(const QString &arg1)
{
    tagChanged = true;
}


void MainWindow::on_importPic_triggered()
{
    auto parDir = QFileDialog::getExistingDirectory(this, "选取文件夹", "");
    QVector<QPair<QString, QString>> files;
    QStack<QString> paths;
    paths.push(parDir);
    while(!paths.empty()){
        auto currDir = QDir(paths.pop());
        QFileInfoList fileInfoList = currDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
        for (auto& fileInfo: fileInfoList) {
           if(fileInfo.isDir())
           {
               paths.push(fileInfo.filePath());
           }
           else if(fileInfo.isFile()){

               files.append(QPair<QString, QString>(fileInfo.canonicalFilePath().replace(parDir, "").replace(fileInfo.fileName(), fileInfo.baseName()),fileInfo.absoluteFilePath()));
           }
       }

    }
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    int size = files.size();
    int i = 0;
    for(auto& file : files){
        dbOperator->tryToSaveImage(file.first, file.second);
        ui->progressBar->setValue(++i / ((float)size) * 100.0);
        qDebug() << "progress is " + QString::number((i / ((float)(size)) * 100.0));
        QApplication::processEvents();
    }
    QMessageBox::information(this, "导入完成", "文件夹中符合要求的图片已经被全部导入！");
    ui->progressBar->setVisible(false);
    updatePlantListDisplay();

}


void MainWindow::on_chooseColor_stateChanged(int arg1)
{
    if(arg1 != 0){
        qDebug() << "choose!";

        QPixmap cursor_pix = QPixmap(":/icons/getColor.png");
        ui->imgLabel->setCursor(QCursor(cursor_pix, 0, 16));
    }
    else if(arg1 == 0){
        qDebug() << "unchoose!";
        ui->imgLabel->setCursor(Qt::ArrowCursor);
    }
}

