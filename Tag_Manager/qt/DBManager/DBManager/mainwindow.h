#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dboperator.h>
#include <QListWidgetItem>
#include <QMouseEvent>
#include "plant.h"
#include "choosenewtagtype.h"
#include "chooseexistdb.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openDB();
    void newDB();
    static void MessageBoxForDebug(QString title, QString content);
    static MainWindow* ptrToMainWindow;
    DBOperator* dbOperator;
    bool isNextNewTagString = true;
    bool isChildDialogAccpet = true;
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    //以下槽函数由QT自动生成，对应窗口中每个部件的相应操作
    void on_newFile_triggered();

    void on_openFile_triggered();

    void on_nameList_currentTextChanged(const QString &currentText);

    void on_tagList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_saveTags_clicked();

    void on_addTag_clicked();

    void on_resetTags_clicked();

    void on_deleteTag_clicked();

    void on_saveName_clicked();

    void on_deletAsset_clicked();

    void on_addAsset_clicked();

    void on_setDBDir_triggered();

    void on_setAssetDir_triggered();

    void on_searchPlant_clicked();

    void on_resetSearch_clicked();

    void on_exportPlant_clicked();

    void on_resetPlantFormwork_2_triggered();

    void on_setPlantFormwork_triggered();

    void on_checkPlantFormwork_triggered();

    void on_KeyText_returnPressed();

    void on_valueText_returnPressed();

    void on_KeyText_textChanged(const QString &arg1);

    void on_valueText_textChanged(const QString &arg1);

    void on_importPic_triggered();

    void on_chooseColor_stateChanged(int arg1);

private:
    bool tagChanged;
    //这一个变量表示窗体中当前显示的植物，在植物列表中切换的时候这一个变量也会切换
    plant currentPlant;
    //这是一个占位符，因为用的是变量不是指针没法赋nullptr，当前植物是这个的时候表示为空
    const plant zombiePlant;
    QVector<QString> currentPlantList;
    //表示当前在窗口中显示的TAG
    plant::tag* currentTag;
    Ui::MainWindow *ui;
    QString DBDir;
    QString assetDir;
    bool imgLabelEnabled = false;
    void chooseDBRootDir();
    //刷新植物列表
    void updatePlantListDisplay();
    //根据当前植物刷新窗体中显示的信息
    void updatePlantDisplay();
    QString readDBBaseDir();
    void writeDBBaseDir(QString dir);
    void setRootDir();
    QString setAssetDir();
    plant* formworkPlant = nullptr;
};
#endif // MAINWINDOW_H
