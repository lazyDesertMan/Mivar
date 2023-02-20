#ifndef CLASS_LIST_H
#define CLASS_LIST_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

class ClassCard : public QWidget{
    Q_OBJECT
};

class ClassList : public QWidget{
    Q_OBJECT
};

class ClassContainer : public QWidget {
    Q_OBJECT

    QLabel* m_header;
    QVBoxLayout* m_layout;
    QScrollArea* m_content;
public:
    explicit ClassContainer(QWidget* parent = nullptr);
    virtual ~ClassContainer() = default;
};

#endif
