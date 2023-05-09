#ifndef RELATIONOPTIONS_H
#define RELATIONOPTIONS_H

#include <QVariant>
#include <QWidget>
#include <QAbstractListModel>
#include "ResetableWidget.h"
#include <models/MivarModel.h>

namespace Ui {
class RelationOptions;
}

class RelationOptions : public QWidget, public ResetableWidget
{
    Q_OBJECT

public:
    explicit RelationOptions(QWidget *parent = nullptr);
    void setEditableRel(std::shared_ptr<MivarModel> model, std::shared_ptr<MivarParam> param);
    void prepareToAddRel(std::shared_ptr<MivarModel> model);
    ~RelationOptions();
    void reset() override;
private slots:
    void onNextClick();
    void onBackClick();
    void onTypeChange(int id);
    // Редактирование отношения
    void editRelation();
    // Добавление отношения
    void addRelation();
private:
    Ui::RelationOptions *ui;
    std::shared_ptr<MivarRelation> m_editableRel;
    std::shared_ptr<MivarRelation> m_relative;
    std::shared_ptr<MivarModel> m_model;
    QWidget* m_activeInput;
    RelationFactory m_relFactory;
};

#endif // RELATIONOPTIONS_H
