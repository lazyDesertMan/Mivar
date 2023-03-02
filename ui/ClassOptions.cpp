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
