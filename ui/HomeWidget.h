#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "ResetableWidget.h"

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();
    void reset() override;
private:
    Ui::HomeWidget *ui;
};

#endif // HOMEWIDGET_H
