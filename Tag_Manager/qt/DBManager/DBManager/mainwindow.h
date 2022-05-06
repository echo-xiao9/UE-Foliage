#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dboperator.h>
#include <QListWidgetItem>
#include "plant.h"
#include "choosenewtagtype.h"
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

private slots:
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

private:
    plant currentPlant;
    const plant zombiePlant;
    QVector<QString> currentPlantList;
    plant::tag* currentTag;
    Ui::MainWindow *ui;
    QString DBDir;
    QString assetDir;
    void chooseDBRootDir();
    void updatePlantListDisplay();
    void updatePlantDisplay();
};
#endif // MAINWINDOW_H
