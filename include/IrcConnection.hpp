#pragma once

#include <QString>
#include <QObject>
#include <QSslSocket>
#include <QMap>
#include <memory>
#include <queue>

/**
 * @class IrcConnection
 * @brief Manages TCP connection to IRC server and message handling
 * 
 * Handles:
 * - Socket lifecycle (connect/disconnect)
 * - Sending/receiving IRC commands
 * - Message buffering
 * - Connection state tracking
 */
class IrcConnection : public QObject {
    Q_OBJECT

public:
    explicit IrcConnection(QObject* parent = nullptr);
    ~IrcConnection();

    // Connection management
    bool connect(const QString& host, quint16 port, bool useTls = false);
    void disconnect();
    bool isConnected() const;
    bool isTlsEnabled() const;
    QString host() const;
    quint16 port() const;

    // Command sending
    void sendCommand(const QString& command);
    void sendMessage(const QString& channel, const QString& message);

    // IRC protocol commands
    void sendPass(const QString& password);
    void sendNick(const QString& nickname);
    void sendUser(const QString& username, const QString& realname);
    void sendInvite(const QString& nickname, const QString& channel);
    void sendMode(const QString& target, const QString& modes = QString());
    void sendJoin(const QString& channel);
    void sendPart(const QString& channel, const QString& reason = QString());
    void sendPrivmsg(const QString& target, const QString& message);
    void sendTopic(const QString& channel, const QString& topic = QString());
    void sendQuit(const QString& reason = QString());

signals:
    void connected();
    void disconnected();
    void connectionError(const QString& error);
    void sslError(const QString& error);
    void messageReceived(const QString& rawMessage);
    void nickChangeRequired();

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onSslErrors(const QList<QSslError>& errors);
    void onEncrypted();
    void onReadyRead();

private:
    void processReceivedData();
    void sendRawCommand(const QString& command);

    std::unique_ptr<QSslSocket> m_socket;
    QString m_nickname;
    QString m_username;
    QString m_host;
    quint16 m_port;
    bool m_useTls;
    QString m_inputBuffer;  // Accumulated incomplete lines
    std::queue<QString> m_commandQueue;  // Outgoing command queue
};
