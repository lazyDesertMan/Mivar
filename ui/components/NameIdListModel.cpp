#include "NameIdListModel.h"

NameIdListModel::NameIdListModel(std::vector<ModelDataType> data, QObject* parent) : m_data(data), QAbstractListModel(parent) {}

int NameIdListModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant NameIdListModel::data(const QModelIndex& index, int role) const {
    QVariant variant;
    if (index.row() < m_data.size()) {
        switch (role)
        {
        case Qt::DisplayRole:
            variant = m_data[index.row()].second;
            break;
        case Qt::UserRole:
            variant = m_data[index.row()].first;
            break;
        default:
            break;
        }
    }
    return variant;
}

int NameIdListModel::rowCount(const QModelIndex& parent) const {
    return m_data.size();
}
