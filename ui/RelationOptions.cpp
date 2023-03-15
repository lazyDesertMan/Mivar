
#include <QObject>
#include <QDebug>
#include <QAction>
#include "RelationOptions.h"
#include "ui_RelationOptions.h"
#include <services/ModelLoader.h>


RelTypeModel::RelTypeModel(std::vector<ModelDataType> data, QObject* parent) : m_data(data), QAbstractListModel(parent) {}

int RelTypeModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant RelTypeModel::data(const QModelIndex& index, int role) const {
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

int RelTypeModel::rowCount(const QModelIndex& parent) const {
    return m_data.size();
}


RelationOptions::RelationOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelationOptions)
{
    ui->setupUi(this);
    RelTypeModel* mod = new RelTypeModel({{MivarRelation::RELATIVE_TYPE_SIMPLE, "Простое"}, {MivarRelation::RELATIVE_TYPE_CONSTRAINT, "Ограничение"},
                                          {MivarRelation::RELATIVE_TYPE_IFCLAUSE, "ifclause"}, {MivarRelation::RELATIVE_TYPE_FUNCTION, "Функция"}});
    ui->typeRelative->setModel(mod);
    //connect(ui->editBtn, SIGNAL(clicked()), this, SLOT(EditRelative()));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(AddRelative()));
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
    ui->nameRelative->setText("");
    ui->descriptionRelative->setText("");
    ui->typeRelative->setCurrentIndex(0);
    ui->editBtn->setVisible(false);
    ui->addBtn->setVisible(true);
}
// Редактирование отношений
void RelationOptions::EditRelative()
{
    QString _name = ui->nameRelative->text();
    QString _desc = ui->descriptionRelative->toPlainText();
    QString _body = ui->bodyRelative->toPlainText();
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

void RelationOptions::AddRelative()
{

}

