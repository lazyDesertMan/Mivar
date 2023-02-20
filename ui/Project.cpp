#include <QLabel>
#include <QStyle>
#include "Project.h"

Project::Project(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    m_classList = new ClassContainer(this);
    m_classList->setMinimumWidth(200);

    QLabel* r = new QLabel(this);
    r->setText("text");

    QSplitter* splitter = new QSplitter(this);
    splitter->addWidget(m_classList);
    splitter->addWidget(r);
    layout->addWidget(splitter);
    layout->setContentsMargins(0, 0, 0, 0);
    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);
    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
    splitter->setContentsMargins(5, 0, 5, 0);
    splitter->setHandleWidth(2);
    splitter->setStyleSheet("QSplitter::handle {background: #555555; margin: 0px 5px 0px 5px};");
}
