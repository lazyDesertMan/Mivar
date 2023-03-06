
#include <QObject>
#include <QDebug>
#include <QAction>
#include "RelationOptions.h"
#include "ui_RelationOptions.h"

RelationOptions::RelationOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelationOptions)
{
    ui->setupUi(this);

    connect(ui->editRelative_Button, SIGNAL(clicked()), this, SLOT(EditRelative()));
}

RelationOptions::~RelationOptions()
{
    delete ui;
}
// Редактирование отношений
void RelationOptions::EditRelative()
{
    QString _name = ui->nameRelative->text();
    QString _desc = ui->descriptionRelative->toPlainText();
    QString _body = ui->bodyRelative->toPlainText();
    QString _type = ui->typeRelative->currentText();
    int index = ui->typeRelative->currentIndex();
    if(index != -1 && _name.size() > 0 && _body.size() > 0){
        // Функция для добавления
        QString test = "Name: " + _name + "; Desc: " + _desc + "; Type: " + _type + " " + QString::number(index) + "; Body: " + _body;
        qDebug() << test;
    }else{
        qDebug() << "Не фортануло";
    }

}

