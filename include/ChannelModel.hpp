#pragma once

#include <QString>
#include <QAbstractListModel>
#include <QMap>
#include <QSet>
#include <memory>

/**
 * @struct Channel
 * @brief Represents an IRC channel
 */
struct Channel {
    QString name;
    QString topic;
    QString modes;
    QSet<QString> users;
    int userCount;
    int unreadCount;

    Channel(const QString& n) 
        : name(n), userCount(0), unreadCount(0) {}
};

/**
 * @class ChannelModel
 * @brief Qt model for channel list
 * 
 * Stores and manages list of joined channels
 * Provides Qt AbstractListModel interface for UI
 */
class ChannelModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit ChannelModel(QObject* parent = nullptr);

    // Qt model interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Channel management
    void addChannel(const QString& name);
    void removeChannel(const QString& name);
    bool hasChannel(const QString& name) const;
    std::shared_ptr<Channel> getChannel(const QString& name);
    std::shared_ptr<const Channel> getChannel(const QString& name) const;

    void setChannelTopic(const QString& name, const QString& topic);
    void setChannelModes(const QString& name, const QString& modes);
    void addUserToChannel(const QString& channel, const QString& user);
    void removeUserFromChannel(const QString& channel, const QString& user);
    void clearChannelUsers(const QString& channel);
    void incrementUnread(const QString& name);
    void clearUnread(const QString& name);
    int unreadCount(const QString& name) const;

    QStringList getChannelNames() const;
    
    void clear();

private:
    QMap<QString, std::shared_ptr<Channel>> m_channels;
};
