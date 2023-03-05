#ifndef TREE_CLASS_DETAIL_H
#define TREE_CLASS_DETAIL_H

#include <QLabel>
#include <QMenu>
#include <QAction>
#include <models/MivarClass.h>

class ClassActions : public QWidget {
    Q_OBJECT
public:
    ClassActions(std::shared_ptr<MivarClass> mivarClass, QWidget* parent = nullptr);
    virtual ~ClassActions() = default;
protected:
    std::shared_ptr<MivarClass> m_class;
    
    QAction* m_editAct;
    QAction* m_removeAct;
    QAction* m_addSubclassAct;
    QAction* m_addParamAct;
    QMenu    m_menu;
    virtual void mousePressEvent(QMouseEvent* event);

private slots:
    void onEditClick();
    void onAddSubclassClick();
    void onAddParamClick();
    void onRemoveClick();
signals:
    void editClick(std::shared_ptr<MivarClass> mivarClass);
    void addSubclassClick(std::shared_ptr<MivarClass> mivarClass);
    void addParamClick(std::shared_ptr<MivarClass> mivarClass);
    void removeClick(std::shared_ptr<MivarClass> mivarClass);
};

class TreeClassDetailType : public ClassActions {
    Q_OBJECT

    QLabel* m_type;
public:
    TreeClassDetailType(std::shared_ptr<MivarClass> mivarClass, QWidget* parent = nullptr);
    ~TreeClassDetailType() = default;
};

class TreeClassDetail : public ClassActions {
    Q_OBJECT

    struct ClassObserver : IObserver {
        TreeClassDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<ClassObserver> m_observer;

    QLabel*  m_icon;
    QLabel*  m_name;
public:
    const std::shared_ptr<MivarClass> getClass() const;
    void update();

    TreeClassDetail(std::shared_ptr<MivarClass> observedClass, QWidget* parent = nullptr);
    virtual ~TreeClassDetail();
};

#endif
