#ifndef MAIN_CONTAINER_H
#define MAIN_CONTAINER_H

#include "QWidget"
#include "QStackedLayout"
#include "Project.h"

class MainContainer : public QWidget {
    Q_OBJECT

    QStackedLayout* m_layout;
    Project* m_project;
public:
    explicit MainContainer(QWidget* parent = nullptr);
    virtual ~MainContainer() = default;
};

#endif
