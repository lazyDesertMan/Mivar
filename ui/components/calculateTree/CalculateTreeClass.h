#ifndef CALCULATE_TREE_CLASS_H
#define CALCULATE_TREE_CLASS_H

#include <QWidget>
#include <QLabel>
#include <models/IObserver.h>
#include <models/MivarModel.h>

class CalculateTreeClassLabel : public QWidget {
    Q_OBJECT

    struct CalculateClassObserver : public IObserver {
        CalculateTreeClassLabel* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<CalculateClassObserver> m_observer;
    std::shared_ptr<MivarClass> m_class;
    QLabel*  m_name;
signals:
    void onParamAdd(std::shared_ptr<MivarClass> mc);
    void onParamRemove(std::shared_ptr<MivarClass> mc);
    void onSubclassAdd(std::shared_ptr<MivarClass> mc);
    void onSubclassRemove(std::shared_ptr<MivarClass> mc);
    void onNameUpdate(std::shared_ptr<MivarClass> mc);
public:
    void paramAdded();
    void paramRemoved();
    void subclassAdded();
    void subclassRemoved();
    void update();

    std::shared_ptr<MivarClass> getClass();

    explicit CalculateTreeClassLabel(std::shared_ptr<MivarClass> observedClass);
    virtual ~CalculateTreeClassLabel();

};

#endif
