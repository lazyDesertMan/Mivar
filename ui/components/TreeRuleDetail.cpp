#include <QBoxLayout>
#include "TreeRuleDetail.h"

TreeRuleDetail::TreeRuleDetail(std::shared_ptr<MivarRule> rule) : TreeRuleActions(), m_rule(rule) {
    m_observer = std::make_shared<RuleObserver>();
    m_observer->parent = this;
    m_rule->addObserver(m_observer);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    m_name = new QLabel(this);

    m_icon = new QLabel(this);
    QPixmap classIcon("://image/res/icons/rule.svg");
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

TreeRuleDetail::~TreeRuleDetail() {
    m_rule->removeObserver(m_observer);
}

void TreeRuleDetail::RuleObserver::handle(int16_t code) {
    if (code == MivarObject::EventCode::EC_NAME_UPDATE) {
        parent->update();
    }
}

void TreeRuleDetail::update() {
    m_name->setText(m_rule->name());
}
