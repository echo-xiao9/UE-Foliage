﻿#ifndef CHOOSENEWTAGTYPE_H
#define CHOOSENEWTAGTYPE_H

#include <QDialog>

namespace Ui {
class ChooseNewTagType;
}

class ChooseNewTagType : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseNewTagType(QWidget *parent = nullptr);
    ~ChooseNewTagType();
    bool stringOrNumberTag();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ChooseNewTagType *ui;
};

#endif // CHOOSENEWTAGTYPE_H
