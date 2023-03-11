#ifndef TREE_RULE_DETAIL
#define TREE_RULE_DETAIL

#include <QWidget>
#include <QLabel>
#include <models/IObserver.h>
#include <models/MivarRule.h>

class TreeRuleActions : public QWidget {
    Q_OBJECT

public:
    
};

class TreeRuleDetail : public TreeRuleActions {
    Q_OBJECT

    struct RuleObserver : IObserver {
        TreeRuleDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<RuleObserver> m_observer;
    std::shared_ptr<MivarRule> m_rule;
    QLabel*  m_icon;
    QLabel*  m_name;
public:
    explicit TreeRuleDetail(std::shared_ptr<MivarRule> rule);
    ~TreeRuleDetail();

    void update();
};

#endif
