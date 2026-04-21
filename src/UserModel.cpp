#include "UserModel.hpp"
#include <algorithm>

UserModel::UserModel(QObject* parent)
    : QAbstractListModel(parent) {}

int UserModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_users.size();
}

QVariant UserModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_users.size()) {
        return QVariant();
    }

    auto it = m_users.begin();
    std::advance(it, index.row());
    auto user = it.value();

    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole: {
            QString display = user->prefix + " " + user->nickname;
            return display;
        }
        case Qt::ToolTipRole:
            if (user->isOperator) {
                return "Channel operator (@)";
            } else if (user->hasVoice) {
                return "Has voice (+)";
            }
            return QString();
        default:
            return QVariant();
    }
}

void UserModel::addUser(const QString& nickname, const QString& prefix) {
    if (hasUser(nickname)) {
        return;
    }

    int row = m_users.size();
    beginInsertRows(QModelIndex(), row, row);
    m_users[nickname] = std::make_shared<User>(nickname, prefix);
    endInsertRows();
}

void UserModel::removeUser(const QString& nickname) {
    auto it = m_users.find(nickname);
    if (it == m_users.end()) {
        return;
    }

    int row = std::distance(m_users.begin(), it);
    beginRemoveRows(QModelIndex(), row, row);
    m_users.erase(it);
    endRemoveRows();
}

bool UserModel::hasUser(const QString& nickname) const {
    return m_users.find(nickname) != m_users.end();
}

std::shared_ptr<User> UserModel::getUser(const QString& nickname) {
    auto it = m_users.find(nickname);
    if (it != m_users.end()) {
        return it.value();
    }
    return nullptr;
}

void UserModel::setUserPrefix(const QString& nickname, const QString& prefix) {
    auto user = getUser(nickname);
    if (user) {
        user->prefix = prefix;
        user->isOperator = prefix.contains('@');
        user->hasVoice = prefix.contains('+');

        int row = std::distance(m_users.begin(), m_users.find(nickname));
        emit dataChanged(index(row), index(row));
    }
}

void UserModel::sortUsers() {
    // Sort: operators first, then voiced users, then regular
    QStringList sorted;
    for (const auto& user : m_users) {
        sorted.push_back(user->nickname);
    }

    std::sort(sorted.begin(), sorted.end(), [this](const QString& a, const QString& b) {
        auto userA = m_users[a];
        auto userB = m_users[b];

        if (userA->isOperator != userB->isOperator) {
            return userA->isOperator;
        }
        if (userA->hasVoice != userB->hasVoice) {
            return userA->hasVoice;
        }
        return a < b;
    });

    // Trigger model reset if order changed
    beginResetModel();
    QMap<QString, std::shared_ptr<User>> newMap;
    for (const auto& nick : sorted) {
        newMap[nick] = m_users[nick];
    }
    m_users = newMap;
    endResetModel();
}

void UserModel::clear() {
    beginResetModel();
    m_users.clear();
    endResetModel();
}

QStringList UserModel::getUserNames() const {
    QStringList names;
    for (auto it = m_users.begin(); it != m_users.end(); ++it) {
        names.push_back(it.key());
    }
    return names;
}
