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

    std::vector<TreeClassDetail*> m_classes;
    std::vector<TreeParamDetail*> m_params;
    std::shared_ptr<MivarModel>   m_model;
public:
    explicit ClassTree(QWidget *parent = nullptr);
    ~ClassTree();
    void DisplayMivar (std::shared_ptr<MivarModel> model);
    void AddChild (QTreeWidgetItem* parentClassItem, const std::shared_ptr<MivarClass>& mivarClass);

private:
    Ui::ClassTree *ui;
};

#endif // CLASSTREE_H
