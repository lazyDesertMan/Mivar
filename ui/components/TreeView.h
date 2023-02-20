#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class TreeView : public QWidget {
    Q_OBJECT

    QLabel* m_header;
    QVBoxLayout* m_mainContainer;
public:
    explicit TreeView(QWidget* parent = nullptr);
    
    QString getText() const noexcept;
    void setText(QString lbl);

    virtual ~TreeView() = default;
};

#endif
