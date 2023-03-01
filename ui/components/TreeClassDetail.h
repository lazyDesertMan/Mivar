#ifndef TREE_CLASS_DETAIL_H
#define TREE_CLASS_DETAIL_H

#include <QLabel>
#include <models/MivarClass.h>

class TreeClassDetail : public QWidget {
    Q_OBJECT

    struct ClassObserver : IObserver {
        TreeClassDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<ClassObserver> m_observer;
    std::shared_ptr<MivarClass> m_class;

    QWidget* m_container;
    QLabel*  m_icon;
    QLabel*  m_name;
public:
    const std::shared_ptr<MivarClass> getClass() const;
    QWidget* name();
    void update();

    TreeClassDetail(std::shared_ptr<MivarClass> observedClass);
    virtual ~TreeClassDetail();
};

#endif
