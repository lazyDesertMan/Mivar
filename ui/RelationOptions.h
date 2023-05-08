#ifndef RELATIONOPTIONS_H
#define RELATIONOPTIONS_H

#include <QVariant>
#include <QWidget>
#include <QAbstractListModel>
#include "ResetableWidget.h"
#include <models/MivarModel.h>



class RelTypeModel : public QAbstractListModel {
    Q_OBJECT
public:
    using ModelDataType = std::pair<QVariant, QVariant>;
    explicit RelTypeModel(std::vector<ModelDataType> data, QObject* parent = 0);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
protected:
    std::vector<ModelDataType> m_data;
};

namespace Ui {
class RelationOptions;
}

class RelationOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit RelationOptions(QWidget *parent = nullptr);
    void setEditableRel(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarParam> param);
    void prepareToAddRel(std::shared_ptr<MivarModel> model);
    ~RelationOptions();
    void reset() override;
private slots:
    void onTypeChange(int);
public slots:
    // Редактирование отношения
    void EditRelative();
    // Добавление отношения
    void AddRelative();
private:
    Ui::RelationOptions *ui;
    std::shared_ptr<MivarRelation> m_editableRel;
    std::shared_ptr<MivarRelation> m_relative;
    std::shared_ptr<MivarModel> m_model;
    QWidget* m_activeInput;
};

#endif // RELATIONOPTIONS_H
