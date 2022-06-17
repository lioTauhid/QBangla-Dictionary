#include "dialogpick.h"
#include "ui_dialogpick.h"
#include <QDebug>
#include <QSettings>

DialogPick::DialogPick(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPick)
{
    ui->setupUi(this);
}

DialogPick::~DialogPick()
{
    delete ui;
}

void DialogPick::on_buttonBox_clicked(QAbstractButton *button)
{
    QSettings setting;
    QString color;
    if(ui->light->isChecked()){
        color= "white";
    }   else if(ui->dark->isChecked()){
        color= "dark";
    }   else{
        color= "default";
    }
    setting.setValue("themColor", color);
    accept();
}

