#include <services/ModelLoader.h>
#include <QDebug>
#include <ui/components/TreeRuleDetail.h>   
#include <ui/components/TreeRelationDetail.h>   
#include "RelationTree.h"
#include "ui_RelationTree.h"

void RelationTree::ClassObserver::bind(std::weak_ptr<MivarClass> observedClass) {
    if (m_isBindet && !m_observedClass.expired())
        m_observedClass.lock()->removeObserver(shared_from_this());
    m_observedClass = observedClass;
    m_observedClass.lock()->addObserver(shared_from_this());
    m_isBindet = true;
}

const std::weak_ptr<MivarClass> RelationTree::ClassObserver::observed() {
    return m_observedClass;
}

void RelationTree::ClassObserver::handle(int16_t code) {
    switch (code) {
    case MivarClass::EventCode::EC_SUBCLASS_REMOVE:
        m_parent->syncModel();
        break;
    case MivarClass::EventCode::EC_RULE_REMOVE:
    case MivarClass::EventCode::EC_RULE_ADD:
        m_parent->updateRules(m_observedClass.lock()->id());
        break;
    }
}

RelationTree::ClassObserver::~ClassObserver() {
    if(m_isBindet && !m_observedClass.expired())
        m_observedClass.lock()->removeObserver(shared_from_this());
}

void RelationTree::ModelObserver::bind(std::weak_ptr<MivarModel> observedModel) {
    if (m_isBindet && !m_observedModel.expired())
        m_observedModel.lock()->removeObserver(shared_from_this());
    m_observedModel = observedModel;
    m_observedModel.lock()->addObserver(shared_from_this());
    m_isBindet = true;
}

const std::weak_ptr<MivarModel> RelationTree::ModelObserver::observed() {
    return m_observedModel;
}

void RelationTree::ModelObserver::handle(int16_t code) {
    switch (code)
    {
    case MivarModel::EventCode::EC_RELATION_ADD:
    case MivarModel::EventCode::EC_RELATION_REMOVE:
        m_parent->updateRelations();
        break;
    }
}

RelationTree::ModelObserver::~ModelObserver() {
    if (m_isBindet && !m_observedModel.expired())
        m_observedModel.lock()->removeObserver(shared_from_this());
}

void RelationTree::syncModel()
{
    // Очистка правил
    for (auto it = m_rules.begin(); it != m_rules.end();) {
        if(m_model->modelClass()->contains(it->first))
            it++;
        else {
            QTreeWidgetItem* ruleItem = it->second.first;
            QTreeWidgetItem* parentItem = ruleItem->parent();
            if (parentItem)
                parentItem->removeChild(ruleItem);
            it = m_rules.erase(it);
        }
    }
    // Очистка подклассов
    for (auto it = m_classes.cbegin(); it != m_classes.cend();) {
        if(m_model->modelClass()->contains(it->first))
            it++;
        else {
            it = m_classes.erase(it);
        }
    }
    
}

void RelationTree::displayRules(std::shared_ptr<MivarClass> mivarClass) {
    m_classes.insert({mivarClass->id(), mivarClass});
    std::shared_ptr<ClassObserver> observer = std::make_shared<ClassObserver>(this);
    observer->bind(mivarClass);
    for (const std::shared_ptr<MivarRule>& rule : mivarClass->rules()) {
        QTreeWidgetItem* parent = m_relations[rule->getBindetRelation()->id()].first;
        QTreeWidgetItem* item = new QTreeWidgetItem();
        parent->addChild(item);
        TreeRuleDetail* details = new TreeRuleDetail(rule);
        m_rules.insert({rule->id(), {item, rule}});
        ui->treeWidget->setItemWidget(item, 0, details);
    }
    for (const std::shared_ptr<MivarClass>& subclass : mivarClass->subclasses())
        displayRules(subclass);
}

RelationTree::RelationTree(QWidget *parent) : QWidget(parent), ui(new Ui::RelationTree)
{
    ui->setupUi(this);
}

void RelationTree::updateRules(const QString& classID) {
    if (m_classes.find(classID) != m_classes.end()) {
        std::shared_ptr<MivarClass> mivarClass = m_classes[classID];
    }
}

void RelationTree::updateRelations() {
    // Очистка отношений
    for (auto it = m_relations.begin(); it != m_relations.end();) {
        if(m_model->)
            it++;
        else {
            QTreeWidgetItem* relItem = it->second.first;
            if (relItem)
                relItem->treeWidget()->dele;
            it = m_relations.erase(it);
        }
    }

    for (const std::shared_ptr<MivarRelation> rel : m_model->relations()){
        if(m_relations.find(rel->id()) != m_relations.end()){
            m_relations.erase(rel->id());
        }
    }
    for (const std::shared_ptr<MivarRelation> rel : m_relations->second{
        if(m_relations.find(rel->id()) != m_relations.end()){
            m_relations.erase(rel->id());
        }
    }
}

void RelationTree::DisplayMivar(std::shared_ptr<MivarModel> model) {
    ui->treeWidget->clear();
    m_classes.clear();
    m_relations.clear();
    m_rules.clear();

    m_model = model;
    std::shared_ptr<ModelObserver> modelObserver = std::make_shared<ModelObserver>(this);
    modelObserver->bind(m_model);
    for(size_t i = 0; i < m_model->relations().size(); i++) {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        ui->treeWidget->addTopLevelItem(item);
        TreeRelationDetail* details = new TreeRelationDetail(model->relations()[i]);
        ui->treeWidget->setItemWidget(item, 0, details);
        m_relations.insert({model->relations()[i]->id(), {item, model->relations()[i]}});
    }
    displayRules(m_model->modelClass());
}

RelationTree::~RelationTree() {
    delete ui;
}
