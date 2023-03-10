#ifndef CLASSTREE_H
#define CLASSTREE_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <models/MivarModel.h>
#include <ui/components/TreeClassDetail.h>
#include <ui/components/TreeParamDetail.h>

namespace Ui {
class ClassTree;
}

class ClassTree : public QWidget
{
    Q_OBJECT

    std::map<QString, std::pair<QTreeWidgetItem*, std::shared_ptr<MivarClass>>> m_classes;
    std::map<QString, std::pair<QTreeWidgetItem*, std::shared_ptr<MivarParam>>> m_params;
    std::shared_ptr<MivarModel> m_model;

    void configureClass(QTreeWidgetItem* mivarClassItem, const std::shared_ptr<MivarClass>& mivarClass, bool isRoot = false);
    QTreeWidgetItem* addClass(QTreeWidgetItem* parent, const std::shared_ptr<MivarClass>& mivarClass);
    void addParam(QTreeWidgetItem* parent, const std::shared_ptr<MivarParam>& mivarParam);

    void clearView();
    void removeClassItem(const std::shared_ptr<MivarClass>& mivarClass);
    
protected slots:
    void deleteClass(const std::shared_ptr<MivarClass>& mivarClass);
    void deleteParam(const std::shared_ptr<MivarParam>& param);
    void editClass(const std::shared_ptr<MivarClass>& mivarClass);
signals:
    void addClassEvent(const std::shared_ptr<MivarClass>& parent);
    void editClassEvent(const std::shared_ptr<MivarClass>& mivarClass, const std::shared_ptr<MivarClass>& parent);
    void addParamEvent(const std::shared_ptr<MivarClass>& mivarClass);
    void editParamEvent(const std::shared_ptr<MivarParam>& mivarParam);
public:
    explicit ClassTree(QWidget *parent = nullptr);
    ~ClassTree();
    void DisplayMivar (std::shared_ptr<MivarModel> model);
    void AddChild (QTreeWidgetItem* parentClassItem, const std::shared_ptr<MivarClass>& mivarClass);
private slots:
    void addParam_slot(std::shared_ptr<MivarClass> mc);
    void addSubclass_slot(std::shared_ptr<MivarClass> mc);
private:
    Ui::ClassTree* ui;
};

#endif // CLASSTREE_H
