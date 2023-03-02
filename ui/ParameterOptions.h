#ifndef PARAMETEROPTIONS_H
#define PARAMETEROPTIONS_H

#include <QWidget>

namespace Ui {
class ParameterOptions;
}

class ParameterOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterOptions(QWidget *parent = nullptr);
    ~ParameterOptions();

private:
    Ui::ParameterOptions *ui;
};

#endif // PARAMETEROPTIONS_H
