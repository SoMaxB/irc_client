#include "IrcNumericHandler.hpp"

#include "MessageParser.hpp"
#include "ChannelModel.hpp"
#include "UserModel.hpp"
#include "IrcConnection.hpp"

#include <QLabel>
#include <QLineEdit>

QString IrcNumericHandler::stripUserPrefix(const QString& nickWithPrefix) {
    QString nick = nickWithPrefix;
    while (!nick.isEmpty() && (nick[0] == '@' || nick[0] == '+' || nick[0] == '%' || nick[0] == '~' || nick[0] == '&')) {
        nick.remove(0, 1);
    }
    return nick;
}

bool IrcNumericHandler::handle(const IrcMessage& msg, const Dependencies& deps) {
    if (!MessageParser::isNumericReply(msg)) {
        return false;
    }

    if (!deps.channelModel || !deps.userModel || !deps.connection || !deps.currentNickname || !deps.currentChannel) {
        return false;
    }

    const int numeric = MessageParser::getNumericCode(msg);
    const QString trailing = msg.trailing;

    if (numeric == 1) {  // RPL_WELCOME
        if (deps.statusBar && deps.currentNickname) {
            deps.statusBar->setText("Registered on server as " + *deps.currentNickname + ". Ready to join channels.");
        }
        if (deps.setRegistrationRequired) {
            deps.setRegistrationRequired(false);
        }
        if (deps.displayBurstLine) {
            QString line = (msg.params.size() >= 2) ? msg.params[1] + " " + trailing : trailing;
            deps.displayBurstLine(line);
        }
        return true;
    }

    if (numeric == 2) {  // RPL_YOURHOST
        if (deps.displayBurstLine) {
            deps.displayBurstLine(trailing);
        }
        return true;
    }

    if (numeric == 3) {  // RPL_CREATED
        if (deps.displayBurstLine) {
            deps.displayBurstLine(trailing);
        }
        return true;
    }

    if (numeric == 4) {  // RPL_MYINFO
        if (deps.displayBurstLine) {
            QString line = (msg.params.size() >= 3) ? msg.params[1] + " " + msg.params[2] : trailing;
            deps.displayBurstLine(line);
        }
        return true;
    }

    if (numeric == 5) {  // RPL_BOUNCE / RPL_ISUPPORT
        if (deps.displayBurstLine) {
            QString line = (msg.params.size() >= 2) ? msg.params[1] + " " + trailing : trailing;
            deps.displayBurstLine(line);
        }
        return true;
    }

    if (numeric == 332) {  // RPL_TOPIC
        if (msg.params.size() >= 3) {
            const QString channel = msg.params[1];
            const QString topic = msg.params[2];
            deps.channelModel->setChannelTopic(channel, topic);
            if (deps.topicLabel && deps.currentChannel && channel == *deps.currentChannel) {
                deps.topicLabel->setText("Topic: " + topic);
            }
        }
        return true;
    }

    if (numeric == 353) {  // RPL_NAMREPLY
        if (msg.params.size() >= 4) {
            const QString channel = msg.params[2];
            const QStringList userList = msg.params[3].split(" ");
            deps.channelModel->clearChannelUsers(channel);
            for (const auto& user : userList) {
                const QString nick = stripUserPrefix(user.trimmed());
                if (!nick.isEmpty()) {
                    deps.channelModel->addUserToChannel(channel, nick);
                }
            }
            if (!deps.currentNickname->isEmpty()) {
                deps.channelModel->addUserToChannel(channel, *deps.currentNickname);
            }
            if (deps.updateUserList && deps.currentChannel && channel == *deps.currentChannel) {
                deps.updateUserList();
            }
        }
        return true;
    }

    if (numeric == 366) {  // RPL_ENDOFNAMES
        deps.userModel->sortUsers();
        return true;
    }

    if (numeric == 376 || numeric == 422) {  // End of MOTD / MOTD missing
        if (deps.statusBar) {
            deps.statusBar->setText("Connected. Use /join #channel to start chatting.");
        }
        return true;
    }

const QString channelOrTarget = msg.params.size() >= 2 ? msg.params[1] : QString();

    if (numeric == 433) {  // ERR_NICKNAMEINUSE
        const QString attemptedNick = (msg.params.size() >= 2) ? msg.params[1] : *deps.currentNickname;
        QString fallbackNick = attemptedNick;
        if (fallbackNick.isEmpty()) {
            fallbackNick = "QtClient";
        }
        fallbackNick += "_";
        if (fallbackNick != *deps.currentNickname) {
            *deps.currentNickname = fallbackNick;
            deps.connection->sendNick(*deps.currentNickname);
            appendNumericSystemMessage(deps, numeric, "Nickname in use. Retrying", *deps.currentNickname, QString());
            if (deps.nickInput) {
                deps.nickInput->setText(*deps.currentNickname);
            }
        }
        return true;
    }

    if (numeric == 401) {  // ERR_NOSUCHNICK
        appendNumericSystemMessage(deps, numeric, "No such nick/channel", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 403) {  // ERR_NOSUCHCHANNEL
        appendNumericSystemMessage(deps, numeric, "No such channel", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 404) {  // ERR_CANNOTSENDTOCHAN
        appendNumericSystemMessage(deps, numeric, "Cannot send to channel", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 421) {  // ERR_UNKNOWNCOMMAND
        appendNumericSystemMessage(deps, numeric, "Unknown command on server", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 451) {  // ERR_NOTREGISTERED
        appendNumericSystemMessage(deps, numeric, "You are not registered yet", channelOrTarget, trailing);
        if (deps.setRegistrationRequired) {
            deps.setRegistrationRequired(true);
        }
        return true;
    }

    if (numeric == 431) {  // ERR_NONICKNAMEGIVEN
        appendNumericSystemMessage(deps, numeric, "Nickname required by server", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 432) {  // ERR_ERRONEUSNICKNAME
        appendNumericSystemMessage(deps, numeric, "Erroneous nickname", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 437) {  // ERR_UNAVAILRESOURCE
        appendNumericSystemMessage(deps, numeric, "Unavailable resource", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 442) {  // ERR_NOTONCHANNEL
        appendNumericSystemMessage(deps, numeric, "You are not on channel", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 443) {  // ERR_USERONCHANNEL
        appendNumericSystemMessage(deps, numeric, "User already on channel", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 462) {  // ERR_ALREADYREGISTERED
        appendNumericSystemMessage(deps, numeric, "Already registered with server", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 464) {  // ERR_PASSWDMISMATCH
        appendNumericSystemMessage(deps, numeric, "Password incorrect or rejected", channelOrTarget, trailing);
        return true;
    }

    if (numeric == 471 || numeric == 473 || numeric == 474 || numeric == 475) {
        appendNumericSystemMessage(deps, numeric, "Cannot join channel", channelOrTarget, trailing.isEmpty() ? QString("Join failed") : trailing);
        if (deps.removeConversation) {
            deps.removeConversation(channelOrTarget, QString(), true);
        }
        return true;
    }

    if (numeric == 405) {  // ERR_TOOMANYCHANNELS
        appendNumericSystemMessage(deps, numeric, "Too many channels: join denied", channelOrTarget, trailing);
        if (deps.removeConversation) {
            deps.removeConversation(channelOrTarget, QString(), true);
        }
        return true;
    }

    if (numeric == 482) {  // ERR_CHANOPRIVSNEEDED
        appendNumericSystemMessage(deps, numeric, "Operator privileges needed", channelOrTarget, trailing);
        return true;
    }

    if (numeric >= 400 && numeric < 600) {
        appendNumericSystemMessage(deps, numeric, "Server error", channelOrTarget, trailing);
        return true;
    }

    return false;
}

QString IrcNumericHandler::composeNumericDetail(const QString& target, const QString& trailing) {
    const QString cleanTarget = target.trimmed();
    const QString cleanTrailing = trailing.trimmed();

    if (!cleanTarget.isEmpty() && !cleanTrailing.isEmpty()) {
        return cleanTarget + " | " + cleanTrailing;
    }

    if (!cleanTarget.isEmpty()) {
        return cleanTarget;
    }

    if (!cleanTrailing.isEmpty()) {
        return cleanTrailing;
    }

    return QString();
}

void IrcNumericHandler::appendNumericSystemMessage(const Dependencies& deps,
                                                   int code,
                                                   const QString& summary,
                                                   const QString& target,
                                                   const QString& trailing) {
    QString message = "[IRC " + QString::number(code) + "] " + summary;
    const QString detail = composeNumericDetail(target, trailing);
    if (!detail.isEmpty()) {
        message += ": " + detail;
    }

    if (deps.appendSystemMessage) {
        deps.appendSystemMessage(message);
    }
}
