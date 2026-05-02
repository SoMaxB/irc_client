#include "MessageParser.hpp"
#include <QStringList>
#include <QDebug>

IrcMessage MessageParser::parse(const QString& line) {
    IrcMessage msg;
    
    if (line.isEmpty()) {
        return msg;  // Invalid empty message
    }

    QString remaining = line;

    // Parse tags @key=value;key2=val2
    if (remaining.startsWith("@")) {
        int spaceIndex = remaining.indexOf(" ");
        if (spaceIndex != -1) {
            QString tagsStr = remaining.mid(1, spaceIndex - 1);
            QStringList pairs = tagsStr.split(";");
            for (const auto& pair : pairs) {
                int eqIndex = pair.indexOf("=");
                if (eqIndex != -1) {
                    msg.tags[pair.left(eqIndex)] = pair.mid(eqIndex + 1);
                }
            }
            remaining = remaining.mid(spaceIndex + 1);
        }
    }

    // Parse source :prefix
    if (remaining.startsWith(":")) {
        int spaceIndex = remaining.indexOf(" ");
        if (spaceIndex != -1) {
            msg.source = remaining.mid(1, spaceIndex - 1);
            remaining = remaining.mid(spaceIndex + 1);
        }
    }

    // Parse command
    int spaceIndex = remaining.indexOf(" ");
    if (spaceIndex == -1) {
        msg.command = remaining.toUpper();
        return msg;
    }

    msg.command = remaining.left(spaceIndex).toUpper();
    remaining = remaining.mid(spaceIndex + 1);

    // Parse parameters and trailing
    while (!remaining.isEmpty()) {
        if (remaining.startsWith(":")) {
            // Everything after : is trailing
            msg.trailing = remaining.mid(1);
            msg.params.push_back(msg.trailing);
            break;
        }

        spaceIndex = remaining.indexOf(" ");
        if (spaceIndex == -1) {
            msg.params.push_back(remaining);
            break;
        }

        msg.params.push_back(remaining.left(spaceIndex));
        remaining = remaining.mid(spaceIndex + 1);
    }

    return msg;
}

QString MessageParser::serialize(const IrcMessage& message) {
    QString result;

    // Tags
    if (!message.tags.isEmpty()) {
        result += "@";
        QStringList tagPairs;
        for (auto it = message.tags.begin(); it != message.tags.end(); ++it) {
            tagPairs.push_back(it.key() + "=" + it.value());
        }
        result += tagPairs.join(";") + " ";
    }

    // Source
    if (!message.source.isEmpty()) {
        result += ":" + message.source + " ";
    }

    // Command
    result += message.command;

    // Parameters and trailing
    if (!message.params.isEmpty()) {
        for (int i = 0; i < message.params.size() - 1; ++i) {
            result += " " + message.params[i];
        }

        // Last parameter - add : if it contains spaces or is trailing
        const QString& lastParam = message.params.last();
        if (lastParam.contains(" ") || lastParam.contains(":")) {
            result += " :" + lastParam;
        } else {
            result += " " + lastParam;
        }
    }

    return result;
}

QString MessageParser::buildPrivmsg(const QString& target, const QString& message) {
    return "PRIVMSG " + target + " :" + message;
}

QString MessageParser::buildNotice(const QString& target, const QString& message) {
    return "NOTICE " + target + " :" + message;
}

QString MessageParser::getCommandType(const IrcMessage& message) {
    return message.command;
}

bool MessageParser::isNumericReply(const IrcMessage& message) {
    if (message.command.length() != 3) {
        return false;
    }
    for (const QChar& c : message.command) {
        if (!c.isDigit()) {
            return false;
        }
    }
    return true;
}

int MessageParser::getNumericCode(const IrcMessage& message) {
    if (!isNumericReply(message)) {
        return -1;
    }
    return message.command.toInt();
}

QMap<QString, QString> MessageParser::parseDccMessage(const QString& trailing) {
    QMap<QString, QString> result;

    QString content = trailing;

    // Handle CTCP-delimited format (\x01DCC SEND...\x01)
    if (content.startsWith(QChar(1))) {
        content = content.mid(1);
        if (content.endsWith(QChar(1))) {
            content.chop(1);
        }
    }

    // Must start with DCC
    if (!content.startsWith("DCC ")) {
        return result;
    }

    content = content.mid(4);

    QStringList parts = content.split(" ");
    if (parts.size() < 4) {
        return result;
    }

    result["type"] = parts[0];
    result["argument"] = parts[1];
    result["address"] = parts[2];
    result["port"] = parts[3];

    if (parts.size() >= 5) {
        result["size"] = parts[4];
    }

    return result;
}

bool MessageParser::isDccSend(const IrcMessage& message) {
    if (message.command != "PRIVMSG") {
        return false;
    }

    if (message.params.size() < 2) {
        return false;
    }

    QMap<QString, QString> dcc = parseDccMessage(message.params[1]);
    return dcc.value("type") == "SEND";
}

QMap<QString, QString> MessageParser::getDccSendDetails(const IrcMessage& message) {
    if (!isDccSend(message)) {
        return QMap<QString, QString>();
    }
    return parseDccMessage(message.params[1]);
}
