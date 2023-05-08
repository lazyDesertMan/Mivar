#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <QWidget>
#include <models/MivarModel.h>
#include "ResetableWidget.h"

namespace Ui {
class RuleOptions;
}

class RuleOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit RuleOptions(QWidget *parent = nullptr);
    ~RuleOptions();
    void prepareToAddRule(std::shared_ptr<MivarModel> model);
    void setEditableRule(std::shared_ptr<MivarModel> model);
    void reset() override;

public slots:
    // Редактирование правила
    void EditRule();
private:
    Ui::RuleOptions *ui;
    std::shared_ptr<MivarModel> m_model;
};

#endif // RULEOPTIONS_H
