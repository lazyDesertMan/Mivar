#include "ClassTree.h"
#include "ui_ClassTree.h"

ClassTree::ClassTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassTree)
{
    ui->setupUi(this);
}

ClassTree::~ClassTree()
{
    delete ui;
}
