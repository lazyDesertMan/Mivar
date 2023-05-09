#include <QObject>
#include <QDebug>
#include <QAction>
#include "RelationOptions.h"
#include "ui_RelationOptions.h"
#include <services/ModelLoader.h>
#include <ui/components/NameIdListModel.h>

void RelationOptions::onNextClick() {
    ui->pagesStack->setCurrentIndex(1);
}

void RelationOptions::onBackClick() {
    ui->pagesStack->setCurrentIndex(0);
}

void RelationOptions::onTypeChange(int /*id*/) {
    int data = ui->typeRelative->currentData(Qt::UserRole).toInt();
    switch (data)
    {
    case MivarRelation::RELATIVE_TYPE_SIMPLE:
    {
        m_activeInput->setVisible(false);
        m_activeInput = ui->simpleCodeEdit;
        m_activeInput->setVisible(true);
        ui->codeInputStack->setCurrentIndex(0);
    }
    break;
    case MivarRelation::RELATIVE_TYPE_CONSTRAINT:
        qDebug() << "Constraint";
        break;
    case MivarRelation::RELATIVE_TYPE_IFCLAUSE:
        qDebug() << "If";
        break;
    case MivarRelation::RELATIVE_TYPE_FUNCTION:
    {
        m_activeInput->setVisible(false);
        m_activeInput = ui->functionCodeEdit;
        m_activeInput->setVisible(true);
        ui->codeInputStack->setCurrentIndex(1);
    }
    break;
    default:
        break;
    }
}

RelationOptions::RelationOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelationOptions)
{
    ui->setupUi(this);
    ui->functionCodeEdit->setTabStopDistance(ui->functionCodeEdit->fontMetrics().horizontalAdvance(' ') * 4);
    NameIdListModel* mod = new NameIdListModel({{MivarRelation::RELATIVE_TYPE_SIMPLE, "Простое"}, {MivarRelation::RELATIVE_TYPE_CONSTRAINT, "Ограничение"},
                                          {MivarRelation::RELATIVE_TYPE_IFCLAUSE, "Условная функция"}, {MivarRelation::RELATIVE_TYPE_FUNCTION, "Функция"}});
    ui->typeRelative->setModel(mod);
    //connect(ui->editBtn, SIGNAL(clicked()), this, SLOT(EditRelative()));
    connect(ui->nextBtn, SIGNAL(clicked()), this, SLOT(onNextClick()));
    connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(onBackClick()));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addRelation()));
    connect(ui->typeRelative, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChange(int)));
}

RelationOptions::~RelationOptions()
{
    delete ui;
}
void RelationOptions::reset()
{
}
/*void RelationOptions::setEditableRel(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarParam> param) {
    m_model = model;
    m_editableParam = param;
    ui->nameParameter->setText(param->name());
    ui->descriptionParameter->setText(param->description());
    ui->listLvlParameter->setEditable(false);
    ui->listLvlParameter->clear();
    if (param->type() == MivarParam::PARAM_TYPE_DOUBLE)
        ui->listTypeDParameter->setCurrentIndex(0);
    else
        ui->listTypeDParameter->setCurrentIndex(1);
    ui->editBtn->setVisible(true);
    ui->addBtn->setVisible(false);
}*/

void RelationOptions::prepareToAddRel(std::shared_ptr<MivarModel> model) {
    m_model = model;
    // Очистка полей ввода
    ui->nameRelative->clear();
    ui->descriptionRelative->clear();
    ui->simpleCodeEdit->clear();
    ui->functionCodeEdit->clear();
    // По умолчанию устанавливается тип отношения "простое"
    ui->typeRelative->setCurrentIndex(0);
    ui->simpleCodeEdit->setVisible(true);
    m_activeInput = ui->simpleCodeEdit;
    ui->functionCodeEdit->setVisible(false);
    
    ui->editBtn->setVisible(false);
    ui->pagesStack->setCurrentIndex(0);
}
// Редактирование отношений
void RelationOptions::editRelation()
{
    QString _name = ui->nameRelative->text();
    QString _desc = ui->descriptionRelative->toPlainText();
    QString _body = ui->simpleCodeEdit->text();
    QString _type = ui->typeRelative->currentText();
    int index = ui->typeRelative->currentIndex();
    if(index != -1 && _name.size() > 0 && _body.size() > 0){
        // Функция для добавления
        QString test = "Name: " + _name + "; Desc: " + _desc + "; Type: " + _type + " " + QString::number(index) + "; Body: " + _body;
        qDebug() << test;
    }else{
        qDebug() << "Не фортануло";
    }

}

void RelationOptions::addRelation()
{
    QString name = ui->nameRelative->text();
    QString desc = ui->descriptionRelative->toPlainText();
    QString code;
    uint type = ui->typeRelative->currentData(Qt::UserRole).toUInt();
    switch (type)
    {
    case MivarRelation::RELATIVE_TYPE_SIMPLE:
        code = ui->simpleCodeEdit->text();
        break;
    /*
    case MivarRelation::RELATIVE_TYPE_CONSTRAINT:
        code = ui->relativeCodeEdit->text();
        break;
    case MivarRelation::RELATIVE_TYPE_IFCLAUSE:
        code = ui->ifclauseCodeEdit->code(); // code() так как придётся пилить отдельный виджет с полями для ввода условия и веток then и else
        break;
    */
    case MivarRelation::RELATIVE_TYPE_FUNCTION:
        code = ui->simpleCodeEdit->text();
        break;
    default:
        break;
    }
    if (name.size()) {
        std::shared_ptr<MivarRelation> rel = m_relFactory(type, name, desc, code);
        if (rel != nullptr) {
            /*
            for (auto input : ui->someInputTable) {
                MivarRelation::RelationParameter param(input.name, input.type);
                rel->addInput(param);
            }
            for (auto output : ui->someOutputTable) {
                MivarRelation::RelationParameter param(output.name, output.type);
                rel->addOutput(param);
            }
            */
        m_model->addRelation(rel);
        }
        /* TODO: Мб добавить ветку else для вывода сообщения в "терминал", что не удалось добавить отношение. Это не обязательно, просто можно подумать */
    }
}

