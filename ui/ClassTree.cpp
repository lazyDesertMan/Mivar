#include "ClassTree.h"
#include "ui_ClassTree.h"
#include <QPushButton>
#include <QDebug>

QTreeWidgetItem* ClassTree::addClass(QTreeWidgetItem* parent, const std::shared_ptr<MivarClass>& mivarClass) {
    QTreeWidgetItem* classItem = new QTreeWidgetItem();
    parent->addChild(classItem);
    
    TreeClassDetail* classDetails = new TreeClassDetail(mivarClass);
    ui->viewTree_Widget->setItemWidget(classItem, 0, classDetails);

    TreeClassDetailType* classDetailType = new TreeClassDetailType(mivarClass);
    ui->viewTree_Widget->setItemWidget(classItem, 1, classDetailType);

    connect(classDetails, &ClassActions::removeClick, this, &ClassTree::deleteClass);
    //connect(classDetails, ClassActions::removeClick, this, deleteClass);
    
    m_classes.push_back({classItem, mivarClass});
    return classItem;
}

void ClassTree::addParam(QTreeWidgetItem* parent, const std::shared_ptr<MivarParam>& mivarParam) {
    QTreeWidgetItem* paramItem = new QTreeWidgetItem();
    parent->addChild(paramItem);
    
    TreeParamDetail* paramDetails = new TreeParamDetail(mivarParam);
    m_params.push_back(paramDetails);
    ui->viewTree_Widget->setItemWidget(paramItem, 0, paramDetails);
    
    paramItem->setText(1, "Параметр");
}

void ClassTree::clearView() {
    m_classes.clear();
    m_params.clear();
    ui->viewTree_Widget->clear();
}

void ClassTree::removeClassItem(const std::shared_ptr<MivarClass>& mivarClass) {
    for(const std::shared_ptr<MivarClass> subclass : mivarClass->subclasses())
        removeClassItem(subclass);
    for(size_t classItemIdx = 0; classItemIdx < m_classes.size(); classItemIdx++)
        if (m_classes[classItemIdx].second->id() == mivarClass->id()) {;
            m_classes.erase(m_classes.begin() + classItemIdx);
            break;
        }
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

    TreeClassDetailType* classDetailType = new TreeClassDetailType(rootClass);
    ui->viewTree_Widget->setItemWidget(item, 1, classDetailType);

    m_classes.push_back({item, rootClass});
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
    for(size_t classItemIdx = 0; classItemIdx < m_classes.size(); classItemIdx++)
        if (m_classes[classItemIdx].second->id() == mivarClass->id()) {
            QTreeWidgetItem* parent = m_classes[classItemIdx].first->parent();
            parent->removeChild(m_classes[classItemIdx].first);
            m_classes.erase(m_classes.begin() + classItemIdx);
            break;
        }
    removeClassItem(mivarClass);
    m_model->modelClass()->removeById(mivarClass->id());
}
