#ifndef TREE_RELATION_DETAIL_H
#define TREE_RELATION_DETAIL_H

#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <models/IObserver.h>
#include <models/MivarRelation.h>

/**
 * @brief Виджет, предоставляющий меню для работы с отношениями
 */
class RelActions : public QWidget {
    Q_OBJECT
public:
    explicit RelActions(std::shared_ptr<MivarRelation> mivarRel);
    virtual ~RelActions() = default;

    const std::shared_ptr<MivarRelation> getRel() const;
protected:
    std::shared_ptr<MivarRelation> m_rel;

    QAction* m_editAct;
    QAction* m_removeAct;
    QAction* m_newRuleAct;
    QAction* m_newRelationAct;
    QMenu    m_menu;
    virtual void mousePressEvent(QMouseEvent* event);

private slots:
    void onEditClick();
    void onRemoveClick();
    void onNewRuleClick();
    void onNewRelationClick();
signals:
    void editClick(std::shared_ptr<MivarRelation> rel);
    void removeClick(std::shared_ptr<MivarRelation> rel);
    void newRuleClick(std::shared_ptr<MivarRelation> rel);
    void newRelationClick();
};

/**
 * @brief Отображение информации о имени отношения
 */
class TreeRelationDetail : public RelActions {
    Q_OBJECT

    struct RelationObserver : IObserver {
        TreeRelationDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<RelationObserver> m_observer;
    QLabel*  m_icon;
    QLabel*  m_name;
public:
    explicit TreeRelationDetail(std::shared_ptr<MivarRelation> relation);
    ~TreeRelationDetail();

    void update();
};

#endif
