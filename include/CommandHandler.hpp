#pragma once

#include <QString>

class CommandHandler {
public:
    enum class Type {
        Join,
        Part,
        Msg,
        Query,
        Me,
        Raw,
        Search,
        Topic,
        Nick,
        Quit,
        Pass,
        User,
        Invite,
        Unknown,
        Invalid
    };

    struct ParsedCommand {
        Type type = Type::Invalid;
        bool valid = false;
        QString commandName;
        QString arg1;
        QString arg2;
        QString errorMessage;
    };

    static ParsedCommand parse(const QString& input);
    static QString supportedCommandsSummary();

private:
    static QString stripOptionalQuotes(QString value);
};
