#ifndef NAME_ID_LIST_MODEL_H
#define NAME_ID_LIST_MODEL_H

#include <QAbstractListModel>

class NameIdListModel : public QAbstractListModel {
    Q_OBJECT
public:
    using ModelDataType = std::pair<QVariant, QVariant>;
    explicit NameIdListModel(std::vector<ModelDataType> data, QObject* parent = 0);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
protected:
    std::vector<ModelDataType> m_data;
};

#endif
