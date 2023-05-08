#include <services/ModelLoader.h>
#include <QDebug>
#include <ui/components/relationsTree/TreeRuleDetail.h>   
#include <ui/components/relationsTree/TreeRelationDetail.h>   
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
        connect(details, &TreeRuleDetail::removeClick, this, &RelationTree::deleteRule);
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
void RelationTree::configureRel(QTreeWidgetItem* mivarRelItem, const std::shared_ptr<MivarRelation>& mivarRel) {
    TreeRelationDetail* relDetails = new TreeRelationDetail(mivarRel);
    ui->treeWidget->setItemWidget(mivarRelItem, 0, relDetails);
    connect(relDetails, &RelActions::removeClick, this, &RelationTree::deleteRelation);
    connect(relDetails, &RelActions::newRelationClick, this, &RelationTree::addRelationEvent);
    connect(relDetails, &RelActions::newRuleClick, this, &RelationTree::addRuleEvent);
}

void RelationTree::deleteRule(const std::shared_ptr<MivarRule>& rule) {
    m_model->modelClass()->removeRule(rule->id());
    syncModel();
}

void RelationTree::deleteRelation(const std::shared_ptr<MivarRelation> &rel)
{
    m_model->removeRelation(rel->id());
}

void RelationTree::updateRules(const QString& classID) {
    if (m_classes.find(classID) != m_classes.end()) {
        std::shared_ptr<MivarClass> mivarClass = m_classes[classID];
        // Очистка отношений
        for (auto it = m_rules.begin(); it != m_rules.end();) {
            if(m_model->modelClass()->contains(it->first))
                it++;
            else {
                QTreeWidgetItem* ruleItem = it->second.first;
                if (ruleItem){
                    QTreeWidgetItem* parent = ruleItem->parent();
                    parent->removeChild(ruleItem);
                }
                it = m_rules.erase(it);
            }
        }
        for (const std::shared_ptr<MivarRule> rule : mivarClass->rules()){
            if(m_rules.find(rule->id()) == m_rules.end()){
                QTreeWidgetItem* item = new QTreeWidgetItem();
                QTreeWidgetItem* parent = m_relations[rule->getBindetRelation()->id()].first;
                parent->addChild(item);
                m_rules.insert({rule->id(), {item, rule}});
            }
        }
    }
}

void RelationTree::updateRelations() {
    // Очистка отношений
    for (auto it = m_relations.begin(); it != m_relations.end();) {
        if(m_model->containsRelation(it->first))
            it++;
        else {
            QTreeWidgetItem* relItem = it->second.first;
            if (relItem){
                int idx = ui->treeWidget->indexOfTopLevelItem(relItem);
                delete ui->treeWidget->takeTopLevelItem(idx);
            }
            it = m_relations.erase(it);
        }
    }
    for (const std::shared_ptr<MivarRelation> rel : m_model->relations()){
        if(m_relations.find(rel->id()) == m_relations.end()){
            QTreeWidgetItem* item = new QTreeWidgetItem();
            ui->treeWidget->addTopLevelItem(item);
            m_relations.insert({rel->id(), {item, rel}});
        }
    }
    syncModel();
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
        configureRel(item, m_model->relations()[i]);
        m_relations.insert({model->relations()[i]->id(), {item, model->relations()[i]}});
    }
    displayRules(m_model->modelClass());
}

RelationTree::~RelationTree() {
    delete ui;
}
