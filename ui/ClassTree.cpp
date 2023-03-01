#include "ClassTree.h"
#include "ui_ClassTree.h"
#include <QPushButton>

ClassTree::ClassTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassTree)
{
    ui->setupUi(this);
    ui->viewTree_Widget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->viewTree_Widget->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->viewTree_Widget->header()->resizeSection(1, 75);
    ui->viewTree_Widget->header()->setStretchLastSection(false);
}

ClassTree::~ClassTree()
{
    delete ui;
}

void ClassTree::DisplayMivar(std::shared_ptr<MivarModel> model){
    m_model = model;
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->viewTree_Widget);
    std::shared_ptr<MivarClass> rootClass = model->modelClass();
    TreeClassDetail* rootClassDetails = new TreeClassDetail(rootClass);
    m_classes.push_back(rootClassDetails);
    ui->viewTree_Widget->setItemWidget(item, 0, rootClassDetails->name());
    item->setText(1, "Класс");
    ui->viewTree_Widget->addTopLevelItem(item);
    for(size_t i = 0; i < rootClass->subclasses().size(); i++){
        AddChild(item, rootClass->subclasses()[i]);
    }
    for(size_t i = 0; i < rootClass->params().size(); i++) {
        QTreeWidgetItem* paramItem = new QTreeWidgetItem();
        TreeParamDetail* paramDetails = new TreeParamDetail(rootClass->params()[i]);
        m_params.push_back(paramDetails);
        ui->viewTree_Widget->setItemWidget(paramItem, 0, paramDetails->name());
        paramItem->setText(1, "Параметр");
        item->addChild(paramItem);
    }
}

void ClassTree::AddChild(QTreeWidgetItem* parentClassItem, const std::shared_ptr<MivarClass>& mivarClass) {
    QTreeWidgetItem* classItem = new QTreeWidgetItem();
    parentClassItem->addChild(classItem);
    TreeClassDetail* classDetails = new TreeClassDetail(mivarClass);
    m_classes.push_back(classDetails);
    ui->viewTree_Widget->setItemWidget(classItem, 0, classDetails->name());
    classItem->setText(1, "Класс");
    for(size_t i = 0; i < mivarClass->subclasses().size(); i++){
        AddChild(classItem, mivarClass->subclasses()[i]);
    }
    for(size_t i = 0; i < mivarClass->params().size(); i++) {
        QTreeWidgetItem* paramItem = new QTreeWidgetItem();
        classItem->addChild(paramItem);
        TreeParamDetail* paramDetails = new TreeParamDetail(mivarClass->params()[i]);
        m_params.push_back(paramDetails);
        ui->viewTree_Widget->setItemWidget(paramItem, 0, paramDetails->name());
        paramItem->setText(1, "Параметр");
    }
}
