#include <QStyle>
#include "TreeView.h"

TreeView::TreeView(QWidget* parent) : QWidget(parent) {
    QColor hoverClr;
    QColor clr = style()->standardPalette().window().color();
    int sumColor = clr.red() + clr.green() + clr.blue();
    if (sumColor < 364)
        hoverClr = QColor::fromRgb(60, 60, 60);
    else
        hoverClr = QColor::fromRgb(155, 155, 155);
    m_header = new QLabel(this);
    m_mainContainer = new QVBoxLayout(this);
    m_mainContainer->addWidget(m_header);
    setLayout(m_mainContainer);
    setStyleSheet("QWidget:hover{ background-color: " + hoverClr.name() + "; }");
    setMaximumHeight(m_header->size().height() + 3);
    setContentsMargins(3, 0, 3, 0);
}

QString TreeView::getText() const noexcept {
    return m_header->text();
}

void TreeView::setText(QString lbl) {
    m_header->setText(lbl);
}
