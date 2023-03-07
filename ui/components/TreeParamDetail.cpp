#include <QMouseEvent>
#include <QBoxLayout>
#include "TreeParamDetail.h"

/*---ParamActions---*/

ParamActions::ParamActions(std::shared_ptr<MivarParam> param) : m_menu(this) {
    m_param = param;

    m_editAct = new QAction("Изменить", this);
    connect(m_editAct, &QAction::triggered, this, &ParamActions::onEditClick);
    m_removeAct = new QAction("Удалить", this);
    connect(m_removeAct, &QAction::triggered, this, &ParamActions::onRemoveClick);
    m_menu.addActions({m_editAct, m_removeAct});
}

const std::shared_ptr<MivarParam> ParamActions::getParam() const {
    return m_param;
}

void ParamActions::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::RightButton)
        m_menu.exec(QCursor::pos());
}

void ParamActions::onEditClick() {
    emit editClick(m_param);
}

void ParamActions::onRemoveClick() {
    emit removeClick(m_param);
}

void TreeParamDetail::update() {
    m_name->setText(m_param->name());
}

/*---TreeParamDetail---*/

TreeParamDetail::TreeParamDetail(std::shared_ptr<MivarParam> observedParam) : ParamActions(observedParam) {
    m_observer = std::make_shared<ParamObserver>();
    m_observer->parent = this;
    m_param = observedParam;
    m_param->addObserver(m_observer);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    m_name = new QLabel(this);
    m_name->setText(m_param->name());

    m_icon = new QLabel(this);
    QPixmap classIcon("://image/res/icons/param.svg");
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

TreeParamDetail::~TreeParamDetail() {
    m_param->removeObserver(m_observer);
}

void TreeParamDetail::ParamObserver::handle(int16_t code) {
    switch (code)
    {
    case MivarObject::EventCode::EC_NAME_UPDATE:
        parent->update();
        break;
    default:
        break;
    }
}

/*---TreeParamType---*/

TreeParamType::TreeParamType(std::shared_ptr<MivarParam> observedParam) : ParamActions(observedParam) {
    m_observer = std::make_shared<ParamObserver>();
    m_observer->parent = this;
    m_param = observedParam;
    m_param->addObserver(m_observer);

    m_type = new QLabel(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    layout->addWidget(m_type);
    layout->setAlignment(Qt::AlignVCenter);

    update();
}

TreeParamType::~TreeParamType() {
    m_param->removeObserver(m_observer);
}

void TreeParamType::ParamObserver::handle(int16_t code) {
    switch (code)
    {
    case MivarParam::EventCode::EC_TYPE_CHANGE:
        parent->update();
        break;
    default:
        break;
    }
}

void TreeParamType::update() {
    switch(m_param->type()) {
        case MivarParam::ParamType::PT_DOUBLE:
            m_type->setText("123");
            break;
        case MivarParam::ParamType::PT_STRING:
            m_type->setText("АБВ");
            break;
        default:
            m_type->setText("Параметр");
            break;
    }
}
