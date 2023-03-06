#include <QDebug>
#include "RuleOptions.h"
#include "ui_RuleOptions.h"

RuleOptions::RuleOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleOptions)
{
    ui->setupUi(this);
    connect(ui->editRule_Button, SIGNAL(clicked()), this, SLOT(EditRule()));
}

RuleOptions::~RuleOptions()
{
    delete ui;
}
// Редактирование правил
void RuleOptions::EditRule()
{
    QString relative = "10 * 11 + ln(12)";
    ui->viewRelative->setText(relative);
    QString _name = ui->nameRule->text();
    QString _desc = ui->descriptionRule->toPlainText();
    if(_name.size() > 0){
        // Функция для добавления
        QString test = "Name: " + _name + "; Desc: " + _desc;
        qDebug() << test;
    }else{
        qDebug() << "Не фортануло";
    }

}
