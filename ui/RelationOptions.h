#ifndef RELATIONOPTIONS_H
#define RELATIONOPTIONS_H

#include <QVariant>
#include <QWidget>
#include <QAbstractListModel>
#include "ResetableWidget.h"
#include <models/MivarModel.h>
#include <ui/components/NameIdListModel.h>

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
    void onNextClick();
    void onBackClick();
    void onTypeChange(int id);
    // Редактирование отношения
    void editRelation();
    // Добавление отношения
    void addRelation();

    void onAddParamClick();
    void onRemoveParamClick();
private:
    static constexpr int PARAM_TYPE_INPUT = 0;
    static constexpr int PARAM_TYPE_OUTPUT = 1;

    Ui::RelationOptions *ui;
    std::shared_ptr<MivarRelation> m_editableRel;
    std::shared_ptr<MivarRelation> m_relative;
    std::shared_ptr<MivarModel> m_model;
    QWidget* m_activeInput;
    RelationFactory m_relFactory;

    std::shared_ptr<NameIdListModel> m_typeSelectorModel;
    std::shared_ptr<NameIdListModel> m_IOSelectorModel;
};

#endif // RELATIONOPTIONS_H
