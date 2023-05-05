#include <algorithm>
#include <QDebug>
#include <QLineEdit>
#include <QCheckBox>
#include "ParamCalculateForm.h"
#include <ui/components/calculateTree/CalculateTreeClass.h>
#include <ui/components/calculateTree/CalculateTreeParam.h>
#include "ui_ParamCalculateForm.h"

void ParamCalculateForm::clearView() {
    ui->paramsTree->clear();
    m_classes.clear();
    m_params.clear();
}

void ParamCalculateForm::configureClass(QTreeWidgetItem *mivarClassItem, const std::shared_ptr<MivarClass> &mivarClass) {
    CalculateTreeClassLabel* classLabel = new CalculateTreeClassLabel(mivarClass);
    ui->paramsTree->setItemWidget(mivarClassItem, 0, classLabel);
    connect(classLabel, &CalculateTreeClassLabel::onSubclassAdd, this, &ParamCalculateForm::onSubclassAdd);
    connect(classLabel, &CalculateTreeClassLabel::onSubclassRemove, this, &ParamCalculateForm::onSubclassDelete);
    connect(classLabel, &CalculateTreeClassLabel::onParamAdd, this, &ParamCalculateForm::onParamAdd);
    connect(classLabel, &CalculateTreeClassLabel::onParamRemove, this, &ParamCalculateForm::onParamDelete);
}

void ParamCalculateForm::configureParam(QTreeWidgetItem *paramItem, const std::shared_ptr<MivarParam> &param) {
    CalculateTreeParamLabel* paramLabel = new CalculateTreeParamLabel(param);
    QLineEdit* input = new QLineEdit();
    QCheckBox* check = new QCheckBox();
    ui->paramsTree->setItemWidget(paramItem, 0, paramLabel);
    ui->paramsTree->setItemWidget(paramItem, 1, input);
    ui->paramsTree->setItemWidget(paramItem, 2, check);
    if (param->defaultValue().length())
        input->setText(param->defaultValue());
}

void ParamCalculateForm::addClass(QTreeWidgetItem *parent, const std::shared_ptr<MivarClass> &mivarClass) {
    QTreeWidgetItem* classItem = new QTreeWidgetItem();
    CalculateTreeClassLabel* parentInfo = static_cast<CalculateTreeClassLabel*>(ui->paramsTree->itemWidget(parent, 0));
    int childIdx = std::min((int)parentInfo->getClass()->subclasses().size() - 1, parent->childCount());
    parent->insertChild(childIdx, classItem);
    configureClass(classItem, mivarClass);
    m_classes.insert({mivarClass->id(), {classItem, mivarClass}});
    for(const std::shared_ptr<MivarClass>& subclass : mivarClass->subclasses())
        addClass(classItem, subclass);
    for(const std::shared_ptr<MivarParam>& param : mivarClass->params())
        addParam(classItem, param);
}

void ParamCalculateForm::addParam(QTreeWidgetItem *parent, const std::shared_ptr<MivarParam> &mivarParam) {
    QTreeWidgetItem* paramItem = new QTreeWidgetItem();
    CalculateTreeClassLabel* parentInfo = static_cast<CalculateTreeClassLabel*>(ui->paramsTree->itemWidget(parent, 0));
    int childIdx = std::min((int)parentInfo->getClass()->subclasses().size() + (int)parentInfo->getClass()->params().size() - 1, parent->childCount());
    parent->insertChild(childIdx, paramItem);
    configureParam(paramItem, mivarParam);
    m_params.insert({mivarParam->id(), {paramItem, mivarParam}});
}

void ParamCalculateForm::deleteStoredClassData(const std::shared_ptr<MivarClass>& mivarClass) {
    m_classes.erase(mivarClass->id());
    for(size_t i = 0; i < mivarClass->params().size(); i++)
        m_params.erase(mivarClass->params()[i]->id());
    for(auto subclass : mivarClass->subclasses())
        deleteStoredClassData(subclass);
}

void ParamCalculateForm::bindWidgets(QTreeWidgetItem *classItem) {
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

std::shared_ptr<MivarClass> ParamCalculateForm::getClassByItem(QTreeWidgetItem* classItem) {
    for(auto curClass : m_classes)
        if(curClass.second.widget == classItem)
            return curClass.second.mivarClass;
    return nullptr;
}

std::shared_ptr<MivarParam> ParamCalculateForm::getParamByItem(QTreeWidgetItem* paramItem) {
    for(auto curParam : m_params)
        if(curParam.second.widget == paramItem)
            return curParam.second.param;
    return nullptr;
}

void ParamCalculateForm::swapChilds(QTreeWidgetItem *item, int firstIdx, int secondIdx) {
    QTreeWidgetItem* secondChild = item->takeChild(secondIdx);
    QTreeWidgetItem* firstChild = item->takeChild(firstIdx);
    item->insertChild(firstIdx, secondChild);
    item->insertChild(secondIdx, firstChild);
}

void ParamCalculateForm::sortTree(QTreeWidgetItem* item) {
    const std::shared_ptr<MivarClass> parentClass = getClassByItem(item);
    const size_t classCnt = parentClass->subclasses().size();
    const size_t paramCnt = parentClass->params().size();
    for(size_t i = 0; i + 1 < classCnt; i++) {
        for(size_t j = i + 1; j < classCnt; j++) {
            const std::shared_ptr<MivarClass> lSubclass = getClassByItem(item->child(i));
            const std::shared_ptr<MivarClass> rSubclass = getClassByItem(item->child(j));
            if (lSubclass != nullptr && rSubclass != nullptr && lSubclass->name().toLower() > rSubclass->name().toLower())
                swapChilds(item, i, j);
        }
    }
    for(size_t i = 0; i + 1 < paramCnt; i++) {
        for(size_t j = i + 1; j < paramCnt; j++) {
            const std::shared_ptr<MivarParam> lParam = getParamByItem(item->child(classCnt + i));
            const std::shared_ptr<MivarParam> rParam = getParamByItem(item->child(classCnt + j));
            if (lParam != nullptr && rParam != nullptr && lParam->name().toLower() > rParam->name().toLower())
                swapChilds(item, classCnt + i, classCnt + j);
        }
    }
    for(size_t i = 0; i < classCnt; i++)
        sortTree(item->child(i));
    bindWidgets(item);
}

void ParamCalculateForm::printCalcResult(const std::vector<ParamCalculator::ParamCalcResult>& results,
                         const std::vector<QString>& outputs,
                         const std::map<QString, QString>& inputs) {
    if (m_browser) {
        m_browser->clear();
        m_browser->setText("Результаты вычислений:");
        for (const ParamCalculator::ParamCalcResult& res : results) {
            if (res.values.size()) {
                QString paramHeader = m_params.at(res.id).param->name() + " = ";
                QString paramValues = "";
                for (size_t i = 0; i < res.values.size(); i++) {
                    if (res.values[i] != m_params.at(res.id).param->defaultValue() && (inputs.find(res.id) == inputs.end() || inputs.at(res.id) != res.values[i]))
                        paramValues += res.values[i] + ", ";
                }
                if (paramValues.length() > 0) {
                    paramHeader += paramValues.remove(paramValues.size() - 2, 2);
                    if (std::find(outputs.begin(), outputs.end(), res.id) != outputs.end())
                        paramHeader = "<div style=\"font-weight: bold; text-decoration: underline;\">" + paramHeader + "</div>";
                    m_browser->append(paramHeader);
                }
            }
        }
    }
}

void ParamCalculateForm::onSubclassAdd(std::shared_ptr<MivarClass> parent) {
    qDebug() << "Subclass add event";
}

void ParamCalculateForm::onSubclassDelete() {
    for (auto curClass : m_classes) {
        if(!m_model->contains(curClass.first)) {
            QTreeWidgetItem* parent = curClass.second.widget->parent();
            parent->removeChild(curClass.second.widget);
            deleteStoredClassData(curClass.second.mivarClass);
        }
    }
}

void ParamCalculateForm::onParamAdd(std::shared_ptr<MivarClass> parent) {
    qDebug() << "Param add event";
}

void ParamCalculateForm::onParamDelete(std::shared_ptr<MivarClass> parent) {
    for (auto curParam : m_params) {
        if(!m_model->contains(curParam.first)) {
            QTreeWidgetItem* parent = curParam.second.widget->parent();
            parent->removeChild(curParam.second.widget);
            m_params.erase(curParam.first);
        }
    }
}

void ParamCalculateForm::onItemExpanded(QTreeWidgetItem* item) {
    ui->paramsTree->resizeColumnToContents(0);
}

void ParamCalculateForm::onCalculateClick() {
    std::map<QString, QString> inputs;
    std::vector<QString> outputs;
    for (auto param : m_params) {
        QLineEdit* paramValInp = static_cast<QLineEdit*>(ui->paramsTree->itemWidget(param.second.widget, 1));
        QCheckBox* isOut = static_cast<QCheckBox*>(ui->paramsTree->itemWidget(param.second.widget, 2));
        if (paramValInp->text().size()) {
            inputs[param.first] = paramValInp->text();
        }
        if (isOut->isChecked())
            outputs.push_back(param.first);
    }
    std::vector<ParamCalculator::ParamCalcResult> results = m_calculator.findWays(m_model, inputs, outputs);
    printCalcResult(results, outputs, inputs);
}

ParamCalculateForm::ParamCalculateForm(QWidget *parent) : QWidget(parent),
                                                          ui(new Ui::ParamCalculateForm)
{
    ui->setupUi(this);
    ui->paramsTree->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->paramsTree->header()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->paramsTree->header()->setSectionResizeMode(2, QHeaderView::Interactive);
    connect(ui->paramsTree, &QTreeWidget::itemExpanded, this, &ParamCalculateForm::onItemExpanded);
    connect(ui->calculateBtn, &QPushButton::clicked, this, &ParamCalculateForm::onCalculateClick);
}

void ParamCalculateForm::setParamOutputWidget(QTextBrowser* browser) {
    m_browser = browser;
}

void ParamCalculateForm::setModel(std::shared_ptr<MivarModel> model) {
    clearView();
    m_model = model;
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->paramsTree);
    std::shared_ptr<MivarClass> rootClass = model->modelClass();
    configureClass(item, rootClass);
    m_classes.insert({rootClass->id(), {item, rootClass}});
    ui->paramsTree->addTopLevelItem(item);
    for(const std::shared_ptr<MivarClass>& subclass : rootClass->subclasses())
        addClass(item, subclass);
    for(const std::shared_ptr<MivarParam> param : rootClass->params())
        addParam(item, param);
    sortTree(item);
    ui->paramsTree->resizeColumnToContents(0);
}

void ParamCalculateForm::reset() {
    m_model.reset();
}

ParamCalculateForm::~ParamCalculateForm()
{
    delete ui;
}
