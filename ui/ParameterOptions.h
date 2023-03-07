#ifndef PARAMETEROPTIONS_H
#define PARAMETEROPTIONS_H

#include <QWidget>
#include <models/MivarClass.h>
#include "ResetableWidget.h"

namespace Ui {
class ParameterOptions;
}

class ParameterOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit ParameterOptions(QWidget *parent = nullptr);
    void setEditableParam(std::shared_ptr<MivarParam> param);
    void reset() override;
    ~ParameterOptions();

public slots:
    // Редактирование параметра
    void EditParameter();

private:
    Ui::ParameterOptions *ui;
    std::shared_ptr<MivarParam> m_editableParam;
};

#endif // PARAMETEROPTIONS_H
