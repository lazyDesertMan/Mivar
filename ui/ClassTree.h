#ifndef CLASSTREE_H
#define CLASSTREE_H

#include <QWidget>

namespace Ui {
class ClassTree;
}

class ClassTree : public QWidget
{
    Q_OBJECT

public:
    explicit ClassTree(QWidget *parent = nullptr);
    ~ClassTree();

private:
    Ui::ClassTree *ui;
};

#endif // CLASSTREE_H
