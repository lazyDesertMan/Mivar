#include "ParameterOptions.h"
#include "ui_ParameterOptions.h"

ParameterOptions::ParameterOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterOptions)
{
    ui->setupUi(this);
}

ParameterOptions::~ParameterOptions()
{
    delete ui;
}
