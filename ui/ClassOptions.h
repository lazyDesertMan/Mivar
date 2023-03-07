#ifndef CLASSOPTIONS_H
#define CLASSOPTIONS_H

#include <QWidget>
#include "ResetableWidget.h"

namespace Ui {
class ClassOptions;
}

class ClassOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit ClassOptions(QWidget *parent = nullptr);
    ~ClassOptions();
    void reset() override;

private slots:
    void on_editClass_Button_clicked();

private:
    Ui::ClassOptions *ui;
};

#endif // CLASSOPTIONS_H
