#include <QDebug>
#include <QUuid>
#include "RuleOptions.h"
#include "ui_RuleOptions.h"

RuleOptions::RuleOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleOptions)
{
    ui->setupUi(this);
    connect(ui->editBtn_Rule, &QPushButton::clicked, this, &RuleOptions::EditRule);
    connect(ui->relationSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChange(int)));
    connect(ui->addBtn_Rule, &QPushButton::clicked, this, &RuleOptions::onAddClick);
}

RuleOptions::~RuleOptions()
{
    delete ui;
}
void RuleOptions::reset()
{
}

void RuleOptions::loadParamsRec(const std::shared_ptr<MivarClass>& mivarClass,
                                std::vector<NameIdListModel::ModelDataType>& numberParams,
                                std::vector<NameIdListModel::ModelDataType>& stringParams)
{
    for(auto curParam : mivarClass->params()) {
        if (curParam->type() == MivarParam::PARAM_TYPE_DOUBLE) {
            numberParams.push_back({curParam->id(), " [" + mivarClass->name() + "]: " + curParam->name()});
        }
        else {
            stringParams.push_back({curParam->id(), " [" + mivarClass->name() + "]: " + curParam->name()});
        }
    }
    for(auto subclass : mivarClass->subclasses()) {
        loadParamsRec(subclass, numberParams, stringParams);
    }
}

void RuleOptions::showParamTable() {
    if(m_class != nullptr) {
        ui->paramBindingTable->clear();
        
    }
}

// Редактирование правил
void RuleOptions::EditRule()
{
    
}

void RuleOptions::prepareToAddRule(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarClass> ruleParentClass) {
    m_model = model;
    m_class = ruleParentClass;
    ui->nameRule->setText("");
    ui->descriptionRule->setText("");
    ui->editBtn_Rule->setVisible(false);
    ui->addBtn_Rule->setVisible(true);
    ui->classNameLine->setText(m_class->name());
    ui->paramBindingTable->resizeColumnsToContents();

    std::vector<NameIdListModel::ModelDataType> relations = {{"", ""}};
    for (auto rel : m_model->relations()) {
        relations.push_back({rel->id(), rel->name()});
    }
    NameIdListModel* relationsModel = new NameIdListModel(relations, ui->relationSelector);
    ui->relationSelector->setModel(relationsModel);

    std::vector<NameIdListModel::ModelDataType> numberParams = {{"", ""}};
    std::vector<NameIdListModel::ModelDataType> stringParams = {{"", ""}};
    loadParamsRec(m_class, numberParams, stringParams);
    m_numberParamModel = std::make_shared<NameIdListModel>(numberParams);
    m_stringParamModel = std::make_shared<NameIdListModel>(stringParams);
}

void RuleOptions::onTypeChange(int /*id*/) {
    ui->paramBindingTable->setRowCount(0);
    QString id = ui->relationSelector->currentData(Qt::UserRole).toString();
    if (id.size()) {
        std::shared_ptr<MivarRelation> rel = m_model->getRelation(id);
        for (auto inParam : rel->inputs()) {
            QComboBox* selector = new QComboBox();
            if (inParam.type == "double") {
                selector->setModel(m_numberParamModel.get());
            }
            else {
                selector->setModel(m_stringParamModel.get());
            }
            int rowIdx = ui->paramBindingTable->rowCount();
            ui->paramBindingTable->insertRow(rowIdx);
            ui->paramBindingTable->setItem(rowIdx, 0, new QTableWidgetItem(inParam.name));
            ui->paramBindingTable->setItem(rowIdx, 1, new QTableWidgetItem("Входной"));
            ui->paramBindingTable->setCellWidget(rowIdx, 2, selector);
        }
        for (auto outParam : rel->outputs()) {
            QComboBox* selector = new QComboBox();
            if (outParam.type == "double") {
                selector->setModel(m_numberParamModel.get());
            }
            else {
                selector->setModel(m_stringParamModel.get());
            }
            int rowIdx = ui->paramBindingTable->rowCount();
            ui->paramBindingTable->insertRow(rowIdx);
            ui->paramBindingTable->setItem(rowIdx, 0, new QTableWidgetItem(outParam.name));
            ui->paramBindingTable->setItem(rowIdx, 1, new QTableWidgetItem("Выходной"));
            ui->paramBindingTable->setCellWidget(rowIdx, 2, selector);
        }
    }
    ui->paramBindingTable->resizeColumnToContents(0);
    ui->paramBindingTable->resizeColumnToContents(1);
}

void RuleOptions::onAddClick() {
    QString id = QUuid::createUuid().toString();
    QString name = ui->nameRule->text();
    QString relationId = ui->relationSelector->currentData(Qt::UserRole).toString();
    QString description = ui->descriptionRule->toPlainText();
    if (name.size() && relationId.size()) {
        std::shared_ptr<MivarRelation> relation = m_model->getRelation(relationId);
        std::shared_ptr<MivarRule> rule = std::make_shared<MivarRule>(id, name, description);
        rule->bindRelation(relation);
        for (int i = 0; i < ui->paramBindingTable->rowCount(); i++) {
            QString paramName = ui->paramBindingTable->item(i, 0)->text();
            QComboBox* selector = static_cast<QComboBox*>(ui->paramBindingTable->cellWidget(i, 2));
            QString id = selector->currentData(Qt::UserRole).toString();
            rule->bindParam(paramName, id);
        }
        if (rule->isCorrect()) {
            m_class->addRule(rule);
        }
    }
}
