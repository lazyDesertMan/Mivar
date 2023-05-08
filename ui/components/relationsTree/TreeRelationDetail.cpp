#include <QDebug>
#include <QMouseEvent>
#include <QBoxLayout>
#include "TreeRelationDetail.h"


RelActions::RelActions(std::shared_ptr<MivarRelation> mivarRel) : m_menu(this) {
    m_rel = mivarRel;

    m_editAct = new QAction("Изменить", this);
    connect(m_editAct, &QAction::triggered, this, &RelActions::onEditClick);
    m_removeAct = new QAction("Удалить", this);
    connect(m_removeAct, &QAction::triggered, this, &RelActions::onRemoveClick);
    m_menu.addActions({m_editAct, m_removeAct});
    m_menu.addSeparator();
}
const std::shared_ptr<MivarRelation> RelActions::getRel() const {
    return m_rel;
}

void RelActions::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::RightButton)
        m_menu.exec(QCursor::pos());
}

void RelActions::onEditClick() {
    emit editClick(m_rel);
}

void RelActions::onRemoveClick() {
    emit removeClick(m_rel);
}

/*
 * TreeRelationDetail
*/

TreeRelationDetail::TreeRelationDetail(std::shared_ptr<MivarRelation> relation) : RelActions(relation) {
    m_observer = std::make_shared<RelationObserver>();
    m_observer->parent = this;
    m_rel->addObserver(m_observer);
    
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
    m_rel->removeObserver(m_observer);
}

void TreeRelationDetail::RelationObserver::handle(int16_t code) {
    if (code == MivarObject::EventCode::EC_NAME_UPDATE) {
        parent->update();
    }
}

void TreeRelationDetail::update() {
    m_name->setText(m_rel->name());
}
