#ifndef RELATIONTREE_H
#define RELATIONTREE_H

#include <map>
#include <QWidget>
#include <QTreeWidgetItem>
#include <models/MivarModel.h>
#include <models/MivarRelation.h>
#include <models/MivarRule.h>

namespace Ui {
class RelationTree;
}

class RelationTree : public QWidget
{
    Q_OBJECT

private:
    class ClassObserver : public IObserver, public std::enable_shared_from_this<ClassObserver> {
        bool m_isBindet;
        std::weak_ptr<MivarClass> m_observedClass;
        RelationTree* m_parent;
    public:
        ClassObserver(RelationTree* parent) : m_isBindet(false), m_parent(parent) {};
        void bind(std::weak_ptr<MivarClass> observedClass);
        const std::weak_ptr<MivarClass> observed();
        void handle(int16_t code) override;
        ~ClassObserver();
    };

    class ModelObserver : public IObserver, public std::enable_shared_from_this<ModelObserver> {
        bool m_isBindet;
        std::weak_ptr<MivarModel> m_observedModel;
        RelationTree* m_parent;
    public:
        ModelObserver(RelationTree* parent) : m_isBindet(false), m_parent(parent) {};
        void bind(std::weak_ptr<MivarModel> observedModel);
        const std::weak_ptr<MivarModel> observed();
        void handle(int16_t code) override;
        ~ModelObserver();
    };

    Ui::RelationTree *ui;

    std::shared_ptr<MivarModel> m_model;
    std::map<QString, std::shared_ptr<MivarClass>> m_classes;
    std::map<QString, std::pair<QTreeWidgetItem*, std::shared_ptr<MivarRelation>>> m_relations;
    std::map<QString, std::pair<QTreeWidgetItem*, std::shared_ptr<MivarRule>>> m_rules;

    void displayRules(std::shared_ptr<MivarClass> mivarClass);

public:
    explicit RelationTree(QWidget* parent = nullptr);
    ~RelationTree();

    void updateRules(QString classID);
    void updateRelations();

    void DisplayMivar(std::shared_ptr<MivarModel> model);
};

#endif // RELATIONTREE_H
