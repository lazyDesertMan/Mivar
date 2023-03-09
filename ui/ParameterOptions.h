#ifndef PARAMETEROPTIONS_H
#define PARAMETEROPTIONS_H

#include <QVariant>
#include <QWidget>
#include <QAbstractListModel>
#include <models/MivarModel.h>
#include <models/MivarClass.h>
#include "ResetableWidget.h"

class ParamTypeModel : public QAbstractListModel {
    Q_OBJECT
public:
    using ModelDataType = std::pair<QVariant, QVariant>;
    explicit ParamTypeModel(std::vector<ModelDataType> data, QObject* parent = 0);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
protected:
    std::vector<ModelDataType> m_data; 
};

namespace Ui {
class ParameterOptions;
}

class ParameterOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit ParameterOptions(QWidget *parent = nullptr);
    void setEditableParam(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarParam> param);
    void prepareToAddParam(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarClass> mivarClass);
    void reset() override;
    ~ParameterOptions();

public slots:
    // Редактирование параметра
    void EditParameter();
    void AddParameter();

private:
    Ui::ParameterOptions *ui;
    std::shared_ptr<MivarParam> m_editableParam;
    std::shared_ptr<MivarClass> m_mivarClass;
    std::shared_ptr<MivarModel> m_model;
};

#endif // PARAMETEROPTIONS_H
