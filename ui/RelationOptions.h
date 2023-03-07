#ifndef RELATIONOPTIONS_H
#define RELATIONOPTIONS_H

#include <QWidget>
#include "ResetableWidget.h"

namespace Ui {
class RelationOptions;
}

class RelationOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit RelationOptions(QWidget *parent = nullptr);
    ~RelationOptions();
    void reset() override;
public slots:
    // Редактирование отношения
    void EditRelative();
private:
    Ui::RelationOptions *ui;
};

#endif // RELATIONOPTIONS_H
