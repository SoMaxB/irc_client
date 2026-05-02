#include "CommandHandler.hpp"

#include <QStringList>

CommandHandler::ParsedCommand CommandHandler::parse(const QString& input) {
    ParsedCommand result;

    if (!input.startsWith('/')) {
        result.errorMessage = "Not a slash command";
        return result;
    }

    const QStringList parts = input.split(' ', Qt::SkipEmptyParts);
    const QString command = parts.value(0).mid(1).toLower();
    result.commandName = command;

    if (command.isEmpty()) {
        result.errorMessage = "Empty command";
        return result;
    }

    if (command == "join") {
        result.type = Type::Join;
        result.arg1 = parts.value(1).trimmed();
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /join #channel";
        }
        return result;
    }

    if (command == "part") {
        result.type = Type::Part;
        result.arg1 = parts.size() > 1 ? parts.value(1).trimmed() : QString();
        result.valid = true;  // explicit channel optional, UI may use current channel
        return result;
    }

    if (command == "msg") {
        result.type = Type::Msg;
        result.arg1 = parts.value(1).trimmed();
        result.arg2 = parts.mid(2).join(" ");
        result.valid = !result.arg1.isEmpty() && !result.arg2.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /msg target message";
        }
        return result;
    }

    if (command == "query") {
        result.type = Type::Query;
        result.arg1 = parts.value(1).trimmed();
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /query nick";
        }
        return result;
    }

    if (command == "me") {
        result.type = Type::Me;
        result.arg1 = parts.mid(1).join(" ");
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /me action";
        }
        return result;
    }

    if (command == "raw") {
        result.type = Type::Raw;
        result.arg1 = parts.mid(1).join(" ");
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /raw IRC command";
        }
        return result;
    }

    if (command == "search") {
        result.type = Type::Search;
        result.arg1 = parts.mid(1).join(" ").trimmed();
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /search text";
        }
        return result;
    }

    if (command == "topic") {
        result.type = Type::Topic;
        result.arg1 = parts.value(1).trimmed();
        result.arg2 = parts.mid(2).join(" ");
        result.valid = true;
        return result;
    }

    if (command == "nick") {
        result.type = Type::Nick;
        result.arg1 = parts.value(1).trimmed();
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /nick newnick";
        }
        return result;
    }

    if (command == "quit") {
        result.type = Type::Quit;
        result.arg1 = parts.mid(1).join(" ");
        result.valid = true;
        return result;
    }

    if (command == "pass") {
        result.type = Type::Pass;
        result.arg1 = parts.value(1).trimmed();
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /pass password";
        }
        return result;
    }

    if (command == "user") {
        result.type = Type::User;
        if (parts.size() < 3) {
            result.errorMessage = "Usage: /user \"username\" \"realname\" (e.g., /user myuser \"My Real Name\")";
            result.valid = false;
            return result;
        }

        QString username = parts.value(1).trimmed();
        QString realname = parts.mid(2).join(" ");
        username = stripOptionalQuotes(username);
        realname = stripOptionalQuotes(realname);

        result.arg1 = username;
        result.arg2 = realname;
        result.valid = !username.isEmpty() && !realname.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /user \"username\" \"realname\"";
        }
        return result;
    }

    if (command == "invite") {
        result.type = Type::Invite;
        result.arg1 = parts.value(1).trimmed();
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /invite nick [#channel]";
        }
        return result;
    }

    if (command == "mode") {
        result.type = Type::Mode;
        result.arg1 = parts.value(1).trimmed();
        result.arg2 = parts.mid(2).join(" ");
        result.valid = !result.arg1.isEmpty();
        if (!result.valid) {
            result.errorMessage = "Usage: /mode #channel [+modes] [params]";
        }
        return result;
    }

    if (command == "dcc") {
        if (parts.size() < 2) {
            result.errorMessage = "Usage: /dcc send nickname filename or /dcc list or /dcc accept id or /dcc reject id";
            result.valid = false;
            return result;
        }

        QString dccSubcommand = parts.value(1).toLower();
        if (dccSubcommand == "send") {
            result.type = Type::DccSend;
            result.arg1 = parts.value(2).trimmed();
            result.arg2 = parts.value(3).trimmed();
            result.valid = !result.arg1.isEmpty() && !result.arg2.isEmpty();
            if (!result.valid) {
                result.errorMessage = "Usage: /dcc send nickname filename";
            }
            return result;
        }

        if (dccSubcommand == "list") {
            result.type = Type::DccList;
            result.valid = true;
            return result;
        }

        if (dccSubcommand == "accept") {
            result.type = Type::DccAccept;
            result.arg1 = parts.value(2).trimmed();
            result.valid = !result.arg1.isEmpty();
            if (!result.valid) {
                result.errorMessage = "Usage: /dcc accept transfer_id";
            }
            return result;
        }

        if (dccSubcommand == "reject") {
            result.type = Type::DccReject;
            result.arg1 = parts.value(2).trimmed();
            result.valid = !result.arg1.isEmpty();
            if (!result.valid) {
                result.errorMessage = "Usage: /dcc reject transfer_id";
            }
            return result;
        }

        result.errorMessage = "Unknown DCC subcommand. Use: send, list, accept, or reject";
        result.valid = false;
        return result;
    }

    result.type = Type::Unknown;
    result.valid = false;
    result.errorMessage = "Unknown command: " + command + ". Available: " + supportedCommandsSummary();
    return result;
}

QString CommandHandler::supportedCommandsSummary() {
    return "/join, /part, /msg, /query, /me, /topic, /nick, /user, /pass, /invite, /mode, /dcc, /raw, /search, /quit";
}

QString CommandHandler::stripOptionalQuotes(QString value) {
    value = value.trimmed();
    if (value.size() >= 2 && value.startsWith('"') && value.endsWith('"')) {
        value = value.mid(1, value.size() - 2);
    }
    return value.trimmed();
}
