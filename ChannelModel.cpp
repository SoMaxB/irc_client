#include "ChannelModel.hpp"
#include <QBrush>
#include <QColor>
#include <QFont>

namespace {

bool isChannelTarget(const QString& name) {
    return name.startsWith('#') ||
           name.startsWith('&') ||
           name.startsWith('+') ||
           name.startsWith('!');
}

}

ChannelModel::ChannelModel(QObject* parent)
    : QAbstractListModel(parent) {}

int ChannelModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_channels.size();
}

QVariant ChannelModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_channels.size()) {
        return QVariant();
    }

    auto it = m_channels.begin();
    std::advance(it, index.row());

    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            if (isChannelTarget(it.value()->name)) {
                QString label = "[CH] " + it.value()->name + " (" + QString::number(it.value()->userCount) + ")";
                if (it.value()->unreadCount > 0) {
                    label += "  {" + QString::number(it.value()->unreadCount) + "}";
                }
                return label;
            }
            if (it.value()->unreadCount > 0) {
                return "[PM] " + it.value()->name + "  {" + QString::number(it.value()->unreadCount) + "}";
            }
            return "[PM] " + it.value()->name;
        case Qt::ToolTipRole:
            if (isChannelTarget(it.value()->name)) {
                return "Channel | Topic: " + it.value()->topic;
            }
            return "Private conversation";
        case Qt::ForegroundRole:
            if (it.value()->unreadCount > 0) {
                // Accent unread conversations in the sidebar.
                return QBrush(QColor("#9cc4ff"));
            }
            return QVariant();
        case Qt::FontRole:
            if (it.value()->unreadCount > 0) {
                QFont font;
                font.setBold(true);
                return font;
            }
            return QVariant();
        default:
            return QVariant();
    }
}

void ChannelModel::addChannel(const QString& name) {
    if (hasChannel(name)) {
        return;
    }

    int row = m_channels.size();
    beginInsertRows(QModelIndex(), row, row);
    m_channels[name] = std::make_shared<Channel>(name);
    endInsertRows();
}

void ChannelModel::removeChannel(const QString& name) {
    auto it = m_channels.find(name);
    if (it == m_channels.end()) {
        return;
    }

    int row = std::distance(m_channels.begin(), it);
    beginRemoveRows(QModelIndex(), row, row);
    m_channels.erase(it);
    endRemoveRows();
}

bool ChannelModel::hasChannel(const QString& name) const {
    return m_channels.find(name) != m_channels.end();
}

std::shared_ptr<Channel> ChannelModel::getChannel(const QString& name) {
    auto it = m_channels.find(name);
    if (it != m_channels.end()) {
        return it.value();
    }
    return nullptr;
}

std::shared_ptr<const Channel> ChannelModel::getChannel(const QString& name) const {
    auto it = m_channels.find(name);
    if (it != m_channels.end()) {
        return it.value();
    }
    return nullptr;
}

void ChannelModel::setChannelTopic(const QString& name, const QString& topic) {
    auto channel = getChannel(name);
    if (channel) {
        channel->topic = topic;
    }
}

void ChannelModel::setChannelModes(const QString& name, const QString& modes) {
    auto channel = getChannel(name);
    if (channel) {
        channel->modes = modes;
    }
}

void ChannelModel::addUserToChannel(const QString& channel, const QString& user) {
    auto ch = getChannel(channel);
    if (ch && !ch->users.contains(user)) {
        ch->users.insert(user);
        ch->userCount++;

        // Notify model of change
        int row = std::distance(m_channels.begin(), m_channels.find(channel));
        emit dataChanged(index(row), index(row));
    }
}

void ChannelModel::removeUserFromChannel(const QString& channel, const QString& user) {
    auto ch = getChannel(channel);
    if (ch && ch->users.contains(user)) {
        ch->users.remove(user);
        ch->userCount--;

        int row = std::distance(m_channels.begin(), m_channels.find(channel));
        emit dataChanged(index(row), index(row));
    }
}

void ChannelModel::clearChannelUsers(const QString& channel) {
    auto ch = getChannel(channel);
    if (ch) {
        ch->users.clear();
        ch->userCount = 0;

        int row = std::distance(m_channels.begin(), m_channels.find(channel));
        emit dataChanged(index(row), index(row));
    }
}

void ChannelModel::incrementUnread(const QString& name) {
    auto channel = getChannel(name);
    if (!channel) {
        return;
    }

    channel->unreadCount++;
    auto it = m_channels.find(name);
    int row = std::distance(m_channels.begin(), it);
    emit dataChanged(index(row), index(row));
}

void ChannelModel::clearUnread(const QString& name) {
    auto channel = getChannel(name);
    if (!channel || channel->unreadCount == 0) {
        return;
    }

    channel->unreadCount = 0;
    auto it = m_channels.find(name);
    int row = std::distance(m_channels.begin(), it);
    emit dataChanged(index(row), index(row));
}

int ChannelModel::unreadCount(const QString& name) const {
    auto channel = getChannel(name);
    if (!channel) {
        return 0;
    }
    return channel->unreadCount;
}

QStringList ChannelModel::getChannelNames() const {
    QStringList names;
    for (auto it = m_channels.begin(); it != m_channels.end(); ++it) {
        names.push_back(it.key());
    }
    return names;
}

void ChannelModel::clear() {
    beginResetModel();
    m_channels.clear();
    endResetModel();
}
