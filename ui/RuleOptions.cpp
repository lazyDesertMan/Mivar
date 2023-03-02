#include "RuleOptions.h"
#include "ui_RuleOptions.h"

RuleOptions::RuleOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleOptions)
{
    ui->setupUi(this);
}

RuleOptions::~RuleOptions()
{
    delete ui;
}
