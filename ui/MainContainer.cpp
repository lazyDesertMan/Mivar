#include "MainContainer.h"
#include <QVBoxLayout>

MainContainer::MainContainer(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    m_project = new Project(this);
    layout->addWidget(m_project);
}
