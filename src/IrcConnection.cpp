#include "IrcConnection.hpp"
#include <QDebug>
#include <QStringList>

IrcConnection::IrcConnection(QObject* parent)
    : QObject(parent), m_socket(std::make_unique<QSslSocket>()), m_port(0), m_useTls(false) {

    QObject::connect(m_socket.get(), &QSslSocket::connected, this, &IrcConnection::onConnected);
    QObject::connect(m_socket.get(), &QSslSocket::disconnected, this, &IrcConnection::onDisconnected);
    QObject::connect(m_socket.get(), &QSslSocket::errorOccurred,
                     this, &IrcConnection::onError);
    QObject::connect(m_socket.get(), &QSslSocket::readyRead, this, &IrcConnection::onReadyRead);
    QObject::connect(m_socket.get(), &QSslSocket::encrypted, this, &IrcConnection::onEncrypted);
    QObject::connect(m_socket.get(), &QSslSocket::sslErrors, this, &IrcConnection::onSslErrors);
}

IrcConnection::~IrcConnection() {
    if (m_socket && m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() == QAbstractSocket::ConnectedState ||
            m_socket->state() == QAbstractSocket::ClosingState) {
            m_socket->waitForDisconnected(500);
        }
    }
}

bool IrcConnection::connect(const QString& host, quint16 port, bool useTls) {
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        qWarning() << "Socket already connected or connecting";
        return false;
    }

    m_host = host;
    m_port = port;
    m_useTls = useTls;

    if (m_useTls) {
        m_socket->connectToHostEncrypted(host, port);
    } else {
        m_socket->connectToHost(host, port);
    }
    
    if (m_useTls) {
        return m_socket->waitForEncrypted(10000);
    }

    return m_socket->waitForConnected(10000);  // 10-second timeout
}

void IrcConnection::disconnect() {
    if (m_socket) {
        m_socket->disconnectFromHost();
        if (m_socket->state() == QAbstractSocket::ConnectedState ||
            m_socket->state() == QAbstractSocket::ClosingState) {
            m_socket->waitForDisconnected(500);
        }
    }
}

bool IrcConnection::isConnected() const {
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

bool IrcConnection::isTlsEnabled() const {
    return m_useTls;
}

QString IrcConnection::host() const {
    return m_host;
}

quint16 IrcConnection::port() const {
    return m_port;
}

void IrcConnection::sendCommand(const QString& command) {
    if (!isConnected()) {
        qWarning() << "Not connected to server";
        return;
    }
    sendRawCommand(command);
}

void IrcConnection::sendMessage(const QString& channel, const QString& message) {
    sendPrivmsg(channel, message);
}

void IrcConnection::sendPass(const QString& password) {
    sendRawCommand("PASS " + password);
}

void IrcConnection::sendNick(const QString& nickname) {
    m_nickname = nickname;
    sendRawCommand("NICK " + nickname);
}

void IrcConnection::sendUser(const QString& username, const QString& realname) {
    m_username = username;
    // USER <username> <hostname> <servername> :<realname>
    sendRawCommand("USER " + username + " 0 * :" + realname);
}

void IrcConnection::sendInvite(const QString& nickname, const QString& channel) {
    sendRawCommand("INVITE " + nickname + " " + channel);
}

void IrcConnection::sendJoin(const QString& channel) {
    sendRawCommand("JOIN " + channel);
}

void IrcConnection::sendPart(const QString& channel, const QString& reason) {
    if (reason.isEmpty()) {
        sendRawCommand("PART " + channel);
    } else {
        sendRawCommand("PART " + channel + " :" + reason);
    }
}

void IrcConnection::sendPrivmsg(const QString& target, const QString& message) {
    sendRawCommand("PRIVMSG " + target + " :" + message);
}

void IrcConnection::sendTopic(const QString& channel, const QString& topic) {
    if (topic.trimmed().isEmpty()) {
        sendRawCommand("TOPIC " + channel);
    } else {
        sendRawCommand("TOPIC " + channel + " :" + topic);
    }
}

void IrcConnection::sendQuit(const QString& reason) {
    if (reason.isEmpty()) {
        sendRawCommand("QUIT");
    } else {
        sendRawCommand("QUIT :" + reason);
    }
}

void IrcConnection::onConnected() {
    qDebug() << "Connected to" << m_host << ":" << m_port;
    if (!m_useTls) {
        emit connected();
    }
}

void IrcConnection::onEncrypted() {
    qDebug() << "TLS handshake completed for" << m_host << ":" << m_port;
    emit connected();
}

void IrcConnection::onDisconnected() {
    qDebug() << "Disconnected from server";
    m_inputBuffer.clear();
    while (!m_commandQueue.empty()) {
        m_commandQueue.pop();
    }
    emit disconnected();
}

void IrcConnection::onError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    QString errorMsg = m_socket->errorString();
    qWarning() << "Socket error:" << errorMsg;
    emit connectionError(errorMsg);
}

void IrcConnection::onSslErrors(const QList<QSslError>& errors) {
    if (errors.isEmpty()) {
        return;
    }

    QStringList descriptions;
    for (const QSslError& error : errors) {
        descriptions << error.errorString();
    }

    const QString errorMsg = descriptions.join("; ");
    qWarning() << "TLS errors:" << errorMsg;
    emit sslError(errorMsg);
    emit connectionError("TLS verification failed: " + errorMsg);
}

void IrcConnection::onReadyRead() {
    if (!m_socket) return;

    m_inputBuffer.append(QString::fromUtf8(m_socket->readAll()));
    processReceivedData();
}

void IrcConnection::processReceivedData() {
    // IRC protocol uses CRLF (\r\n) as line terminator
    // Split on CRLF and process complete lines
    
    int crlfIndex;
    while ((crlfIndex = m_inputBuffer.indexOf("\r\n")) != -1) {
        QString line = m_inputBuffer.left(crlfIndex);
        m_inputBuffer.remove(0, crlfIndex + 2);  // Remove line + CRLF

        if (!line.isEmpty()) {
            emit messageReceived(line);
        }
    }
}

void IrcConnection::sendRawCommand(const QString& command) {
    if (!isConnected()) {
        qWarning() << "Cannot send command: not connected";
        return;
    }

    QString fullCommand = command + "\r\n";
    qint64 written = m_socket->write(fullCommand.toUtf8());
    
    if (written == -1) {
        qWarning() << "Failed to send command:" << command;
    } else {
        m_socket->flush();
        qDebug() << "Sent:" << command;
    }
}
