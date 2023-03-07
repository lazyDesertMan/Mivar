#ifndef TREE_PARAM_DETAIL_H
#define TREE_PARAM_DETAIL_H

#include <QLabel>
#include <QMenu>
#include <QAction>
#include <models/MivarClass.h>

/**
 * @brief Виджет, предоставляющий меню для работы с параметрами миварных классов дочерним виджетам
 */
class ParamActions : public QWidget {
    Q_OBJECT
public:
    explicit ParamActions(std::shared_ptr<MivarParam> param);
    virtual ~ParamActions() = default;

    const std::shared_ptr<MivarParam> getParam() const;
protected:
    std::shared_ptr<MivarParam> m_param;
    
    QAction* m_editAct;
    QAction* m_removeAct;
    QMenu    m_menu;
    virtual void mousePressEvent(QMouseEvent* event);

private slots:
    void onEditClick();
    void onRemoveClick();
signals:
    void editClick(std::shared_ptr<MivarParam> param);
    void removeClick(std::shared_ptr<MivarParam> param);
};

/**
 * @brief Отображение информации о имени параметра
 */
class TreeParamDetail : public ParamActions {
    Q_OBJECT

    struct ParamObserver : IObserver {
        TreeParamDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<ParamObserver> m_observer;

    QLabel*  m_icon;
    QLabel*  m_name;
public:
    void update();

    explicit TreeParamDetail(std::shared_ptr<MivarParam> observedParam);
    ~TreeParamDetail();
};

/**
 * @brief Отображение информации о типе параметра
 */
class TreeParamType : public ParamActions {
    Q_OBJECT

    struct ParamObserver : IObserver {
        TreeParamType* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<ParamObserver> m_observer;

    QLabel* m_type;
public:
    void update();

    explicit TreeParamType(std::shared_ptr<MivarParam> observedParam);
    ~TreeParamType();
};

#endif
