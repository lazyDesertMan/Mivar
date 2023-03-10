#ifndef CLASSOPTIONS_H
#define CLASSOPTIONS_H

#include <QWidget>
#include "ResetableWidget.h"
#include <models/MivarClass.h>

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
    void setEditableClass(std::shared_ptr<MivarClass> mc, const std::shared_ptr<MivarClass>& parent);
    void prepareToAddClass(std::shared_ptr<MivarClass> mc);

private slots:
    // Редактирование класса
    void EditClass();
    void AddClass();

private:
    Ui::ClassOptions *ui;
    std::shared_ptr<MivarClass> m_mivarClass;
};

#endif // CLASSOPTIONS_H
