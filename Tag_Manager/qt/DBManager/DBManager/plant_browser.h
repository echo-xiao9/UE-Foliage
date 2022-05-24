#ifndef PLANT_BROWSER_H
#define PLANT_BROWSER_H

#include <QDialog>
#include "plant.h"

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
