
#include <iostream>
#include <QDebug>
#include "ClassOptions.h"
#include "ui_ClassOptions.h"

ClassOptions::ClassOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassOptions)
{
    ui->setupUi(this);
}

ClassOptions::~ClassOptions()
{
    delete ui;
}

// Редактирование классов
void ClassOptions::on_editClass_Button_clicked()
{
    QString _name = ui->ClassName->text();
    QString _desc = ui->ClassDescroption->toPlainText();
    QString _lvl = ui->ClassLvl->currentText();
    int indexLvl = ui->ClassLvl->currentIndex();
    if(indexLvl != -1 && _name.size() > 0){
        // Функция для добавления
        QString test = "Name: " + _name + "; Desc: " + _desc + "; Lvl: " + _lvl + " index: " + QString::number(indexLvl);
        qDebug() << test;
    }else{
        qDebug() << "Не фортануло";
    }

}

