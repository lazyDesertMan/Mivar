#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <QWidget>

namespace Ui {
class RuleOptions;
}

class RuleOptions : public QWidget
{
    Q_OBJECT

public:
    explicit RuleOptions(QWidget *parent = nullptr);
    ~RuleOptions();

private:
    Ui::RuleOptions *ui;
};

#endif // RULEOPTIONS_H
