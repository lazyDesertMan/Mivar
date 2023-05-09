#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <QWidget>
#include <models/MivarModel.h>
#include "ResetableWidget.h"
#include <ui/components/NameIdListModel.h>

namespace Ui {
class RuleOptions;
}

class RuleOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit RuleOptions(QWidget *parent = nullptr);
    ~RuleOptions();
    void prepareToAddRule(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarClass> ruleParentClass);
    void setEditableRule(std::shared_ptr<MivarModel> model);
    void reset() override;

private:
    void loadParamsRec(const std::shared_ptr<MivarClass>& mivarClass,
                       std::vector<NameIdListModel::ModelDataType>& numberParams,
                       std::vector<NameIdListModel::ModelDataType>& stringParams);
    void showParamTable();

    Ui::RuleOptions *ui;
    std::shared_ptr<MivarModel> m_model;
    std::shared_ptr<MivarClass> m_class;
    std::shared_ptr<NameIdListModel> m_stringParamModel;
    std::shared_ptr<NameIdListModel> m_numberParamModel;

public slots:
    // Редактирование правила
    void EditRule();

private slots:
    void onTypeChange(int id);
    void onAddClick();
};

#endif // RULEOPTIONS_H
