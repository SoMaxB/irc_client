#pragma once

#include <QString>
#include <QAbstractListModel>
#include <QMap>
#include <memory>

/**
 * @struct User
 * @brief Represents an IRC user in current channel
 */
struct User {
    QString nickname;
    QString prefix;  // @ for op, + for voice, etc
    bool isOperator;
    bool hasVoice;

    User(const QString& nick, const QString& pfx = "")
        : nickname(nick), prefix(pfx), isOperator(false), hasVoice(false) {
        isOperator = pfx.contains('@');
        hasVoice = pfx.contains('+');
    }
};

/**
 * @class UserModel
 * @brief Qt model for user list in current channel
 * 
 * Maintains list of users in the currently selected channel
 * Provides sorting and filtering
 */
class UserModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit UserModel(QObject* parent = nullptr);

    // Qt model interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // User management
    void addUser(const QString& nickname, const QString& prefix = "");
    void removeUser(const QString& nickname);
    bool hasUser(const QString& nickname) const;
    std::shared_ptr<User> getUser(const QString& nickname);

    void setUserPrefix(const QString& nickname, const QString& prefix);
    void sortUsers();
    void clear();

    QStringList getUserNames() const;

private:
    QMap<QString, std::shared_ptr<User>> m_users;
    QMap<QString, int> m_userIndices;  // For maintaining sort order
};
