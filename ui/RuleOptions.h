#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <QWidget>
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
    void reset() override;
public slots:
    // Редактирование правила
    void EditRule();
private:
    Ui::RuleOptions *ui;
};

#endif // RULEOPTIONS_H
