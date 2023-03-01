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

void ClassTree::DisplayMivar(MivarModel model){
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->viewTree_Widget);
    MivarClass MC = model.modelClass();
    item->setText(0, MC.name());
    item->setText(1, MC.description());
    for(size_t i = 0; i < MC.subclasses().size(); i++){
        AddChild(item, MC.subclasses()[i]);
    }
    ui->viewTree_Widget->addTopLevelItem(item);
}

void ClassTree::AddChild(QTreeWidgetItem *item, MivarClass MC)
{
    QTreeWidgetItem* childItem = new QTreeWidgetItem();
    childItem->setText(0, MC.name());
    childItem->setText(1, MC.description());
    for(size_t i = 0; i < MC.subclasses().size(); i++){
        AddChild(childItem, MC.subclasses()[i]);
    }
    item->addChild(childItem);
}
