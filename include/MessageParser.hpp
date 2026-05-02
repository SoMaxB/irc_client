#pragma once

#include <QString>
#include <QMap>
#include <QVector>

/**
 * @struct IrcMessage
 * @brief Parsed IRC protocol message
 */
struct IrcMessage {
    QMap<QString, QString> tags;      // @key=value;key2=val2
    QString source;                    // prefix without ':'
    QString command;                   // always uppercase (JOIN, PRIVMSG, etc)
    QVector<QString> params;           // all parameters including trailing
    QString trailing;                  // last parameter (message content, etc)

    bool isValid() const { return !command.isEmpty(); }
};

/**
 * @class MessageParser
 * @brief Parses and serializes IRC protocol messages
 * 
 * Implements RFC 1459 IRC protocol parsing:
 * [tags] [source] command [params] [:trailing]
 */
class MessageParser {
public:
    /**
     * @brief Parse a complete IRC message line (without CRLF)
     * @param line Raw IRC line
     * @return Parsed IrcMessage or invalid if parsing fails
     */
    static IrcMessage parse(const QString& line);

    /**
     * @brief Serialize an IrcMessage back to protocol format
     * @param message The message to serialize
     * @return String with command (no CRLF)
     */
    static QString serialize(const IrcMessage& message);

    /**
     * @brief Build a PRIVMSG command
     */
    static QString buildPrivmsg(const QString& target, const QString& message);

    /**
     * @brief Build a NOTICE command
     */
    static QString buildNotice(const QString& target, const QString& message);

    /**
     * @brief Extract command type for routing
     */
    static QString getCommandType(const IrcMessage& message);

    /**
     * @brief Check if this is a numeric reply (001, 404, etc)
     */
    static bool isNumericReply(const IrcMessage& message);

    /**
     * @brief Get numeric code if isNumericReply is true
     */
    static int getNumericCode(const IrcMessage& message);

    /**
     * @brief Parse a CTCP DCC message from PRIVMSG trailing
     * @param trailing The trailing parameter (with \x01 delimiters)
     * @return Map with parsed DCC data or empty if invalid
     */
    static QMap<QString, QString> parseDccMessage(const QString& trailing);

    /**
     * @brief Check if this is a DCC SEND request
     */
    static bool isDccSend(const IrcMessage& message);

    /**
     * @brief Extract DCC SEND details from message
     */
    static QMap<QString, QString> getDccSendDetails(const IrcMessage& message);
};
