#ifndef PLANT_BROWSER_H
#define PLANT_BROWSER_H

#include <QDialog>
#include "plant.h"
/*这个窗口只用来显示现在的模板，不过也可以根据传入的plant显示整个植物的tag信息*/
namespace Ui {
class plant_browser;
}

class plant_browser : public QDialog
{
    Q_OBJECT

public:
    explicit plant_browser(QWidget *parent = nullptr);
    ~plant_browser();
    void setPlant(plant* plant);

private:
    Ui::plant_browser *ui;
};

#endif // PLANT_BROWSER_H
