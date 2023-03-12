#ifndef TREE_RULE_DETAIL
#define TREE_RULE_DETAIL

#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <models/IObserver.h>
#include <models/MivarRule.h>

/**
 * @brief Виджет, предоставляющий меню для работы с отношениями
 */
class RuleActions : public QWidget {
    Q_OBJECT
public:
    explicit RuleActions(std::shared_ptr<MivarRule> mivarRule);
    virtual ~RuleActions() = default;

    const std::shared_ptr<MivarRule> getRule() const;
protected:
    std::shared_ptr<MivarRule> m_rule;

    QAction* m_editAct;
    QAction* m_removeAct;
    QMenu    m_menu;
    virtual void mousePressEvent(QMouseEvent* event);

private slots:
    void onEditClick();
    void onRemoveClick();
signals:
    void editClick(std::shared_ptr<MivarRule> rule);
    void removeClick(std::shared_ptr<MivarRule> rule);
};

/**
 * @brief Отображение информации о имени правила
 */
class TreeRuleDetail : public RuleActions {
    Q_OBJECT

    struct RuleObserver : IObserver {
        TreeRuleDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<RuleObserver> m_observer;
    QLabel*  m_icon;
    QLabel*  m_name;
public:
    explicit TreeRuleDetail(std::shared_ptr<MivarRule> rule);
    ~TreeRuleDetail();

    void update();
};

#endif
