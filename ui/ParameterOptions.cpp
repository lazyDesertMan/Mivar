#include <QUuid>
#include <QDebug>
#include <QTextStream>
#include "ParameterOptions.h"
#include "ui_ParameterOptions.h"

ParamTypeModel::ParamTypeModel(std::vector<ModelDataType> data, QObject* parent) : m_data(data), QAbstractListModel(parent) {}

int ParamTypeModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant ParamTypeModel::data(const QModelIndex& index, int role) const {
    QVariant variant;
    if (index.row() < m_data.size()) {
        switch (role)
        {
        case Qt::DisplayRole:
            variant = m_data[index.row()].second;
            break;
        case Qt::UserRole:
            variant = m_data[index.row()].first;
            break;
        default:
            break;
        }
    }
    return variant;
}

int ParamTypeModel::rowCount(const QModelIndex& parent) const {
    return m_data.size();
}

ParameterOptions::ParameterOptions(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ParameterOptions)
{
    ui->setupUi(this);
    ParamTypeModel* mod = new ParamTypeModel({{MivarParam::PARAM_TYPE_DOUBLE, "Число"}, {MivarParam::PARAM_TYPE_STRING, "Строка"}});
    ui->listTypeDParameter->setModel(mod);
    connect(ui->editBtn, SIGNAL(clicked()), this, SLOT(EditParameter()));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(AddParameter()));
}

void ParameterOptions::setEditableParam(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarParam> param) {
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
}

void ParameterOptions::prepareToAddParam(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarClass> mivarClass) {
    m_model = model;
    m_mivarClass = mivarClass;
    ui->nameParameter->setText("");
    ui->descriptionParameter->setText("");
    ui->listLvlParameter->setEditable(false);
    ui->listLvlParameter->clear();
    ui->listLvlParameter->addItem(mivarClass->name());
    ui->listTypeDParameter->setCurrentIndex(0);
    ui->editBtn->setVisible(false);
    ui->addBtn->setVisible(true);
}

void ParameterOptions::reset() {
    m_editableParam.reset();
    m_mivarClass.reset();
}

ParameterOptions::~ParameterOptions() {
    delete ui;
}

void ParameterOptions::AddParameter() {
    if (ui->nameParameter->text().size() > 0) {
        QString id = QUuid::createUuid().toString();
        QString name = ui->nameParameter->text();
        QString description = ui->descriptionParameter->toPlainText();
        std::shared_ptr<MivarParam> param = std::make_shared<MivarParam>();
    }
}

// Редактирование параметров
void ParameterOptions::EditParameter() {
    const int16_t type = ui->listTypeDParameter->currentData(Qt::UserRole).toInt();
    if(ui->nameParameter->text().size())
        m_editableParam->setName(ui->nameParameter->text());
    if(ui->descriptionParameter->toPlainText().size())
        m_editableParam->setDescription(ui->descriptionParameter->toPlainText());
    if(m_editableParam->type() != type)
        m_editableParam->setType(type);
}
