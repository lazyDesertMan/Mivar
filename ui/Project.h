#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include "ClassList.h"

class Project : public QWidget {
    Q_OBJECT

    QVBoxLayout* m_layout;
    QSplitter* m_splitter;
    ClassContainer* m_classList;
public:
    explicit Project(QWidget* parent = nullptr);
    virtual ~Project() = default;
};

#endif
