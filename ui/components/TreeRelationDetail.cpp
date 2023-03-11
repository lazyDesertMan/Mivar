#include <QBoxLayout>
#include "TreeRelationDetail.h"

TreeRelationDetail::TreeRelationDetail(std::shared_ptr<MivarRelation> relation) : TreeRelationActions(), m_relation(relation) {
    m_observer = std::make_shared<RelationObserver>();
    m_observer->parent = this;
    m_relation->addObserver(m_observer);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    m_name = new QLabel(this);

    m_icon = new QLabel(this);
    QPixmap classIcon("://image/res/icons/relation.svg");
    m_icon->setPixmap(classIcon);
    m_icon->setScaledContents(true);
    m_icon->setFixedSize(16, 16);
    m_icon->setAlignment(Qt::AlignVCenter);

    layout->addWidget(m_icon);
    layout->addWidget(m_name);
    layout->setStretch(0, 0);
    layout->setStretch(1, 1);
    layout->setAlignment(Qt::AlignVCenter);

    update();
}

TreeRelationDetail::~TreeRelationDetail() {
    m_relation->removeObserver(m_observer);
}

void TreeRelationDetail::RelationObserver::handle(int16_t code) {
    if (code == MivarObject::EventCode::EC_NAME_UPDATE) {
        parent->update();
    }
}

void TreeRelationDetail::update() {
    m_name->setText(m_relation->name());
}
