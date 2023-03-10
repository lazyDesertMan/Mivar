#ifndef TREE_CLASS_DETAIL_H
#define TREE_CLASS_DETAIL_H

#include <QLabel>
#include <QMenu>
#include <QAction>
#include <models/MivarClass.h>

/**
 * @brief Виджет, предоставляющий меню для работы с миварными классами дочерним виджетам
 */
class ClassActions : public QWidget {
    Q_OBJECT
public:
    explicit ClassActions(std::shared_ptr<MivarClass> mivarClass, bool isRootClass = false);
    virtual ~ClassActions() = default;

    const std::shared_ptr<MivarClass> getClass() const;
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

/**
 * @brief Отображение информации о типе миварного класса
 */
class TreeClassDetailType : public ClassActions {
    Q_OBJECT

    QLabel* m_type;
public:
    explicit TreeClassDetailType(std::shared_ptr<MivarClass> mivarClass, bool isRootClass = false);
    ~TreeClassDetailType() = default;
};

/**
 * @brief Отображение информации о имени миварного класса
 */
class TreeClassDetail : public ClassActions {
    Q_OBJECT

    struct ClassObserver : IObserver {
        TreeClassDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<ClassObserver> m_observer;
    QLabel*  m_icon;
    QLabel*  m_name;
signals:
    void onParamAdd(std::shared_ptr<MivarClass> mc);
public:
    void paramAdded();
    void update();

    explicit TreeClassDetail(std::shared_ptr<MivarClass> observedClass, bool isRootClass = false);
    virtual ~TreeClassDetail();
};

#endif
