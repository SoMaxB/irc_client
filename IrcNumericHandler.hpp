#pragma once

#include <QString>
#include <QStringList>
#include <functional>

class ChannelModel;
class UserModel;
class IrcConnection;
class QLineEdit;
class QLabel;
struct IrcMessage;

class IrcNumericHandler {
public:
    struct Dependencies {
        ChannelModel* channelModel = nullptr;
        UserModel* userModel = nullptr;
        IrcConnection* connection = nullptr;
        QLineEdit* nickInput = nullptr;
        QLabel* statusBar = nullptr;
        QLabel* topicLabel = nullptr;
        QString* currentNickname = nullptr;
        const QString* currentChannel = nullptr;
        std::function<void(const QString&)> appendSystemMessage;
        std::function<void(const QString&, const QString&, bool)> removeConversation;
        std::function<void()> updateUserList;
        std::function<void(bool)> setRegistrationRequired;
        std::function<void(const QString&)> displayBurstLine;
    };

    static bool handle(const IrcMessage& msg, const Dependencies& deps);

private:
    static QString composeNumericDetail(const QString& target, const QString& trailing);
    static void appendNumericSystemMessage(const Dependencies& deps,
                                          int code,
                                          const QString& summary,
                                          const QString& target = QString(),
                                          const QString& trailing = QString());
    static QString stripUserPrefix(const QString& nickWithPrefix);
};
