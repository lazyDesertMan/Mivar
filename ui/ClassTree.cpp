#include "ClassTree.h"
#include "ui_ClassTree.h"
#include <QPushButton>
#include <QDebug>

void ClassTree::configureClass(QTreeWidgetItem* mivarClassItem, const std::shared_ptr<MivarClass>& mivarClass, bool isRoot) {
    TreeClassDetail* classDetails = new TreeClassDetail(mivarClass, isRoot);
    ui->viewTree_Widget->setItemWidget(mivarClassItem, 0, classDetails);
    connect(classDetails, &ClassActions::removeClick, this, &ClassTree::deleteClass);
    connect(classDetails, &ClassActions::editClick, this, &ClassTree::editClass);
    connect(classDetails, &ClassActions::addSubclassClick, this, &ClassTree::addClassEvent);
    connect(classDetails, &ClassActions::addParamClick, this, &ClassTree::addParamEvent);
    connect(classDetails, &TreeClassDetail::onParamAdd, this, &ClassTree::addParam_slot);
    connect(classDetails, &TreeClassDetail::onSubclassAdd, this, &ClassTree::addSubclass_slot);
    connect(classDetails, &ClassActions::addRelClick, this, &ClassTree::addRelEvent);
    connect(classDetails, &TreeClassDetail::onNameUpdate, this, &ClassTree::sortClass);
    //connect(classDetails, &TreeClassDetail::onRelAdd, this, &ClassTree::addRel_slot);

    TreeClassDetailType* classDetailType = new TreeClassDetailType(mivarClass, isRoot);
    ui->viewTree_Widget->setItemWidget(mivarClassItem, 1, classDetailType);
    connect(classDetailType, &ClassActions::removeClick, this, &ClassTree::deleteClass);
    connect(classDetailType, &ClassActions::editClick, this, &ClassTree::editClass);
    connect(classDetailType, &ClassActions::addSubclassClick, this, &ClassTree::addClassEvent);
    connect(classDetailType, &ClassActions::addParamClick, this, &ClassTree::addParamEvent);
    connect(classDetailType, &ClassActions::addRelClick, this, &ClassTree::addRelEvent);
}

void ClassTree::configureParam(QTreeWidgetItem *paramItem, const std::shared_ptr<MivarParam> &param) {
    TreeParamDetail* paramDetails = new TreeParamDetail(param);  
    ui->viewTree_Widget->setItemWidget(paramItem, 0, paramDetails);
    connect(paramDetails, &ParamActions::removeClick, this, &ClassTree::deleteParam);
    connect(paramDetails, &ParamActions::editClick, this, &ClassTree::editParamEvent);

    TreeParamType* paramType = new TreeParamType(param);
    ui->viewTree_Widget->setItemWidget(paramItem, 1, paramType);
    connect(paramType, &ParamActions::removeClick, this, &ClassTree::deleteParam);
    connect(paramType, &ParamActions::editClick, this, &ClassTree::editParamEvent);
}

QTreeWidgetItem *ClassTree::addClass(QTreeWidgetItem *parent, const std::shared_ptr<MivarClass> &mivarClass)
{
    QTreeWidgetItem* classItem = new QTreeWidgetItem();
    TreeClassDetail* parentInfo = static_cast<TreeClassDetail*>(ui->viewTree_Widget->itemWidget(parent, 0));
    int childIdx = std::min((int)parentInfo->getClass()->subclasses().size() - 1, parent->childCount());
    parent->insertChild(childIdx, classItem);
    configureClass(classItem, mivarClass);
    m_classes.insert({mivarClass->id(), {classItem, mivarClass}});
    return classItem;
}

void ClassTree::addParam(QTreeWidgetItem* parent, const std::shared_ptr<MivarParam>& mivarParam) {
    QTreeWidgetItem* paramItem = new QTreeWidgetItem();
    parent->addChild(paramItem);
    configureParam(paramItem, mivarParam);
    m_params.insert({mivarParam->id(), {paramItem, mivarParam}});
}

void ClassTree::clearView() {
    m_classes.clear();
    m_params.clear();
    ui->viewTree_Widget->clear();
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
    configureClass(item, rootClass, true);
    m_classes.insert({rootClass->id(), {item, rootClass}});
    ui->viewTree_Widget->addTopLevelItem(item);
    for(const std::shared_ptr<MivarClass>& subclass : rootClass->subclasses())
        AddChild(item, subclass);
    for(const std::shared_ptr<MivarParam> param : rootClass->params())
        addParam(item, param);
    sortTree(item);
}

void ClassTree::AddChild(QTreeWidgetItem* parentClassItem, const std::shared_ptr<MivarClass>& mivarClass) {
    QTreeWidgetItem* classItem = addClass(parentClassItem, mivarClass);
    for(const std::shared_ptr<MivarClass>& subclass : mivarClass->subclasses())
        AddChild(classItem, subclass);
    for(const std::shared_ptr<MivarParam>& param : mivarClass->params())
        addParam(classItem, param);
}

void ClassTree::addParam_slot(std::shared_ptr<MivarClass> mc)
{
    QTreeWidgetItem* classItem = m_classes[mc->id()].first;
    for (size_t idx = 0; idx < mc->params().size(); idx++) {
        if (m_params.find(mc->params()[idx]->id()) == m_params.end())
            addParam(classItem, mc->params()[idx]);
    }
}
void ClassTree::addSubclass_slot(std::shared_ptr<MivarClass> mc)
{
    std::pair<QTreeWidgetItem*, std::shared_ptr<MivarClass>> classItem = m_classes[mc->id()];
    for (size_t idx = 0; idx < classItem.second->subclasses().size(); idx++) {
        if (m_classes.find(classItem.second->subclasses()[idx]->id()) == m_classes.end())
            addClass(classItem.first, classItem.second->subclasses()[idx]);
    }
    sortTree(classItem.first);
}

void ClassTree::bindWidgets(QTreeWidgetItem* classItem) {
    const std::shared_ptr<MivarClass> parentClass = getClassByItem(classItem);
    const size_t subclassCnt = parentClass->subclasses().size();
    const size_t paramCnt = parentClass->params().size();
    for(size_t i = 0; i < subclassCnt; i++) {
        configureClass(classItem->child(i), getClassByItem(classItem->child(i)));
        bindWidgets(classItem->child(i));
    }
    for(size_t i = 0; i < paramCnt; i++)
        configureParam(classItem->child(subclassCnt + i), getParamByItem(classItem->child(subclassCnt + i)));
}

void ClassTree::deleteStoredClassData(const std::shared_ptr<MivarClass> &mivarClass)
{
    m_classes.erase(mivarClass->id());
    for(size_t i = 0; i < mivarClass->params().size(); i++)
        m_params.erase(mivarClass->params()[i]->id());
    for(auto subclass : mivarClass->subclasses())
        deleteStoredClassData(subclass);
}

std::shared_ptr<MivarClass> ClassTree::getClassByItem(QTreeWidgetItem* classItem) {
    for(auto curClass : m_classes)
        if(curClass.second.first == classItem)
            return curClass.second.second;
    return nullptr;
}
std::shared_ptr<MivarParam> ClassTree::getParamByItem(QTreeWidgetItem *paramItem) {
    for(auto curParam : m_params)
        if(curParam.second.first == paramItem)
            return curParam.second.second;
    return nullptr;
};

void ClassTree::swapChilds(QTreeWidgetItem* item, int firstIdx, int secondIdx) {
    ui->viewTree_Widget->removeItemWidget(item->child(firstIdx), 0);
    ui->viewTree_Widget->removeItemWidget(item->child(firstIdx), 1);
    ui->viewTree_Widget->removeItemWidget(item->child(secondIdx), 0);
    ui->viewTree_Widget->removeItemWidget(item->child(secondIdx), 1);
    // Порядок удаления и вставки потомков не менять
    QTreeWidgetItem* secondChild = item->takeChild(secondIdx);
    QTreeWidgetItem* firstChild = item->takeChild(firstIdx);
    item->insertChild(firstIdx, secondChild);
    item->insertChild(secondIdx, firstChild);
}

void ClassTree::sortTree(QTreeWidgetItem* item) {
    const std::shared_ptr<MivarClass> parentClass = getClassByItem(item);
    const size_t classCnt = parentClass->subclasses().size();
    const size_t paramCnt = parentClass->params().size();
    for(size_t i = 0; i + 1 < classCnt; i++) {
        for(size_t j = i + 1; j < classCnt; j++) {
            const std::shared_ptr<MivarClass> lSubclass = getClassByItem(item->child(i));
            const std::shared_ptr<MivarClass> rSubclass = getClassByItem(item->child(j));
            if (lSubclass->name().toLower() > rSubclass->name().toLower())
                swapChilds(item, i, j);
        }
    }
    for(size_t i = 0; i + 1 < paramCnt; i++) {
        for(size_t j = i + 1; j < paramCnt; j++) {
            const std::shared_ptr<MivarParam> lParam = getParamByItem(item->child(classCnt + i));
            const std::shared_ptr<MivarParam> rParam = getParamByItem(item->child(classCnt + j));
            if (lParam->name().toLower() > rParam->name().toLower())
                swapChilds(item, classCnt + i, classCnt + j);
        }
    }
    for(size_t i = 0; i < classCnt; i++)
        sortTree(item->child(i));
    bindWidgets(item);
}

void ClassTree::deleteClass(const std::shared_ptr<MivarClass>& mivarClass) {
    if(m_classes.find(mivarClass->id()) != m_classes.end()) {
        QTreeWidgetItem* parent = m_classes[mivarClass->id()].first->parent();
        parent->removeChild(m_classes[mivarClass->id()].first);
    }
    deleteStoredClassData(mivarClass);
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

void ClassTree::editClass(const std::shared_ptr<MivarClass> &mivarClass) {
    QTreeWidgetItem* item = m_classes[mivarClass->id()].first->parent();
    if (item == nullptr)
        emit editClassEvent(mivarClass, nullptr);
    else
        for(std::pair<QString, std::pair<QTreeWidgetItem*, std::shared_ptr<MivarClass>>> i:m_classes){
            if(i.second.first == item){
                emit editClassEvent(mivarClass, i.second.second);
                return;
            }
        }
}

void ClassTree::sortClass(std::shared_ptr<MivarClass> mc) {
    QTreeWidgetItem* parent = m_classes[mc->id()].first->parent();
    if (parent != nullptr)
        sortTree(parent);
}
