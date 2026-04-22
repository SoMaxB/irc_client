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
