#ifndef CLASSTREE_H
#define CLASSTREE_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <models/MivarModel.h>


namespace Ui {
class ClassTree;
}

class ClassTree : public QWidget
{
    Q_OBJECT

public:
    explicit ClassTree(QWidget *parent = nullptr);
    ~ClassTree();
    void DisplayMivar (MivarModel model);
    void AddChild (QTreeWidgetItem* item, MivarClass MC);


private:
    Ui::ClassTree *ui;
};

#endif // CLASSTREE_H
