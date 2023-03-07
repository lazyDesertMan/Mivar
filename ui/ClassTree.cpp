#include "ClassTree.h"
#include "ui_ClassTree.h"
#include <QPushButton>
#include <QDebug>

QTreeWidgetItem* ClassTree::addClass(QTreeWidgetItem* parent, const std::shared_ptr<MivarClass>& mivarClass) {
    QTreeWidgetItem* classItem = new QTreeWidgetItem();
    parent->addChild(classItem);
    
    TreeClassDetail* classDetails = new TreeClassDetail(mivarClass);
    ui->viewTree_Widget->setItemWidget(classItem, 0, classDetails);
    connect(classDetails, &ClassActions::removeClick, this, &ClassTree::deleteClass);
    connect(classDetails, &ClassActions::editClick, this, &ClassTree::editClassEvent);
    connect(classDetails, &ClassActions::addSubclassClick, this, &ClassTree::addClassEvent);

    TreeClassDetailType* classDetailType = new TreeClassDetailType(mivarClass);
    ui->viewTree_Widget->setItemWidget(classItem, 1, classDetailType);
    connect(classDetailType, &ClassActions::removeClick, this, &ClassTree::deleteClass);
    connect(classDetailType, &ClassActions::editClick, this, &ClassTree::editClassEvent);
    connect(classDetailType, &ClassActions::addSubclassClick, this, &ClassTree::addClassEvent);
    
    m_classes.insert({mivarClass->id(), {classItem, mivarClass}});
    return classItem;
}

void ClassTree::addParam(QTreeWidgetItem* parent, const std::shared_ptr<MivarParam>& mivarParam) {
    QTreeWidgetItem* paramItem = new QTreeWidgetItem();
    parent->addChild(paramItem);
    
    TreeParamDetail* paramDetails = new TreeParamDetail(mivarParam);
    m_params.insert({mivarParam->id(), {paramItem, mivarParam}});
    ui->viewTree_Widget->setItemWidget(paramItem, 0, paramDetails);
    connect(paramDetails, &ParamActions::removeClick, this, &ClassTree::deleteParam);
    connect(paramDetails, &ParamActions::editClick, this, &ClassTree::editParamEvent);
    
    TreeParamType* paramType = new TreeParamType(mivarParam);
    m_params.insert({mivarParam->id(), {paramItem, mivarParam}});
    ui->viewTree_Widget->setItemWidget(paramItem, 1, paramType);
    connect(paramType, &ParamActions::removeClick, this, &ClassTree::deleteParam);
    connect(paramType, &ParamActions::editClick, this, &ClassTree::editParamEvent);
}

void ClassTree::clearView() {
    m_classes.clear();
    m_params.clear();
    ui->viewTree_Widget->clear();
}

void ClassTree::removeClassItem(const std::shared_ptr<MivarClass>& mivarClass) {
    for(const std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        removeClassItem(subclass);
    m_classes.erase(mivarClass->id());
}

ClassTree::ClassTree(QWidget *parent) : QWidget(parent),
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

void ClassTree::DisplayMivar(std::shared_ptr<MivarModel> model) {
    clearView();
    m_model = model;
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->viewTree_Widget);
    std::shared_ptr<MivarClass> rootClass = model->modelClass();

    TreeClassDetail* rootClassDetails = new TreeClassDetail(rootClass);
    ui->viewTree_Widget->setItemWidget(item, 0, rootClassDetails);
    connect(rootClassDetails, &ClassActions::editClick, this, &ClassTree::editClassEvent);
    connect(rootClassDetails, &ClassActions::addSubclassClick, this, &ClassTree::addClassEvent);

    TreeClassDetailType* classDetailType = new TreeClassDetailType(rootClass);
    ui->viewTree_Widget->setItemWidget(item, 1, classDetailType);
    connect(classDetailType, &ClassActions::editClick, this, &ClassTree::editClassEvent);
    connect(classDetailType, &ClassActions::addSubclassClick, this, &ClassTree::addClassEvent);

    m_classes.insert({rootClass->id(), {item, rootClass}});
    ui->viewTree_Widget->addTopLevelItem(item);
    for(const std::shared_ptr<MivarClass>& subclass : rootClass->subclasses())
        AddChild(item, subclass);
    for(const std::shared_ptr<MivarParam> param : rootClass->params())
        addParam(item, param);
}

void ClassTree::AddChild(QTreeWidgetItem* parentClassItem, const std::shared_ptr<MivarClass>& mivarClass) {
    QTreeWidgetItem* classItem = addClass(parentClassItem, mivarClass);
    for(const std::shared_ptr<MivarClass>& subclass : mivarClass->subclasses())
        AddChild(classItem, subclass);
    for(const std::shared_ptr<MivarParam>& param : mivarClass->params())
        addParam(classItem, param);
}

void ClassTree::deleteClass(const std::shared_ptr<MivarClass>& mivarClass) {
    if(m_classes.find(mivarClass->id()) != m_classes.end()) {
        QTreeWidgetItem* parent = m_classes[mivarClass->id()].first->parent();
        parent->removeChild(m_classes[mivarClass->id()].first);
        m_classes.erase(mivarClass->id());
    }
    for(size_t i = 0; i < mivarClass->params().size(); i++)
        m_params.erase(mivarClass->params()[i]->id());
    removeClassItem(mivarClass);
    m_model->modelClass()->removeById(mivarClass->id());
}

void ClassTree::deleteParam(const std::shared_ptr<MivarParam>& param) {
    if(m_params.find(param->id()) != m_params.end()) {
        QTreeWidgetItem* parent = m_params[param->id()].first->parent();
        parent->removeChild(m_params[param->id()].first);
        m_params.erase(param->id());
    }
    m_model->modelClass()->removeById(param->id());
}
