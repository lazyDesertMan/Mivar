#include <QLabel>
#include <QPalette>
#include <QStyle>
#include "ClassList.h"
#include "components/TreeView.h"

ClassContainer::ClassContainer(QWidget *parent) : QWidget(parent) {
    m_header = new QLabel(this);
    m_header->setText("Модель");
    m_header->setAlignment(Qt::AlignCenter);

    QPalette p;
    QColor clr = style()->standardPalette().window().color();
    int sumColor = clr.red() + clr.green() + clr.blue();
    if (sumColor < 364)
        p.setColor(QPalette::Window, QColor::fromRgb(70, 70, 70));
    else
        p.setColor(QPalette::Window, QColor::fromRgb(165, 165, 165));
    setAutoFillBackground(true);
    setPalette(p);

    m_content = new QScrollArea(this);
    m_content->setStyleSheet("QScrollArea { border: 0px; };");
    TreeView* tree = new TreeView(this);
    tree->setText("Header");
    m_content->setWidget(tree);
    m_content->setWidgetResizable(true);
    m_content->setContentsMargins(0, 0, 0, 0);

    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_layout->addWidget(m_header);
    m_layout->addWidget(m_content);
    m_layout->setStretch(0, 0);
    m_layout->setStretch(1, 1);
    m_layout->setContentsMargins(0, 0, 0, 0);
}