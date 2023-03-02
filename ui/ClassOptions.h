#ifndef CLASSOPTIONS_H
#define CLASSOPTIONS_H

#include <QWidget>

namespace Ui {
class ClassOptions;
}

class ClassOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ClassOptions(QWidget *parent = nullptr);
    ~ClassOptions();

private:
    Ui::ClassOptions *ui;
};

#endif // CLASSOPTIONS_H
