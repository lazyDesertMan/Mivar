
#include <QDebug>
#include "ParameterOptions.h"
#include "ui_ParameterOptions.h"

ParameterOptions::ParameterOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterOptions)
{
    ui->setupUi(this);
    connect(ui->editParameter_Button, SIGNAL(clicked()), this, SLOT(EditParameter()));
}

ParameterOptions::~ParameterOptions()
{
    delete ui;
}

// Редактирование параметров
void ParameterOptions::EditParameter()
{
    QString _name = ui->nameParameter->text();
    QString _desc = ui->descriptionParameter->toPlainText();
    QString _typeD = ui->listTypeDParameter->currentText();
    int indexTD = ui->listTypeDParameter->currentIndex();
    QString _lvl = ui->listLvlParameter->currentText();
    int indexLvl = ui->listLvlParameter->currentIndex();
    QString _value = ui->valParameter->text();
    if(indexTD != -1 && indexLvl != -1 && _name.size() > 0 && _value.size() > 0){
        // Функция для добавления
        QString test = "Name: " + _name + "; Desc: " + _desc + "; TypeD: " + _typeD + " " + QString::number(indexTD) + "; Lvl: " + _lvl + " " + QString::number(indexLvl) + "; Value: " + _value;
        qDebug() << test;
    }else{
        qDebug() << "Не фортануло";
    }

}
