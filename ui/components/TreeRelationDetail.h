#ifndef TREE_RELATION_DETAIL_H
#define TREE_RELATION_DETAIL_H

#include <QWidget>
#include <QLabel>
#include <models/IObserver.h>
#include <models/MivarRelation.h>

class TreeRelationActions : public QWidget {
    Q_OBJECT

public:
    
};

class TreeRelationDetail : public TreeRelationActions {
    Q_OBJECT

    struct RelationObserver : IObserver {
        TreeRelationDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<RelationObserver> m_observer;
    std::shared_ptr<MivarRelation> m_relation;
    QLabel*  m_icon;
    QLabel*  m_name;
public:
    explicit TreeRelationDetail(std::shared_ptr<MivarRelation> relation);
    ~TreeRelationDetail();

    void update();
};

#endif
