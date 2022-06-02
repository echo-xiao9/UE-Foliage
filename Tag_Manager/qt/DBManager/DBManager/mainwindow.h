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

private:
    bool tagChanged;
    plant currentPlant;
    const plant zombiePlant;
    QVector<QString> currentPlantList;
    plant::tag* currentTag;
    Ui::MainWindow *ui;
    QString DBDir;
    QString assetDir;
    bool imgLabelEnabled = false;
    void chooseDBRootDir();
    void updatePlantListDisplay();
    void updatePlantDisplay();
    QString readDBBaseDir();
    void writeDBBaseDir(QString dir);
    void setRootDir();
    QString setAssetDir();
    plant* formworkPlant = nullptr;
};
#endif // MAINWINDOW_H
