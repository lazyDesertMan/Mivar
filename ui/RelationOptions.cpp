#include "RelationOptions.h"
#include "ui_RelationOptions.h"

RelationOptions::RelationOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelationOptions)
{
    ui->setupUi(this);
}

RelationOptions::~RelationOptions()
{
    delete ui;
}
