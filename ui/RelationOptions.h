#ifndef RELATIONOPTIONS_H
#define RELATIONOPTIONS_H

#include <QWidget>

namespace Ui {
class RelationOptions;
}

class RelationOptions : public QWidget
{
    Q_OBJECT

public:
    explicit RelationOptions(QWidget *parent = nullptr);
    ~RelationOptions();
public slots:
    // Редактирование отношения
    void EditRelative();
private:
    Ui::RelationOptions *ui;
};

#endif // RELATIONOPTIONS_H
