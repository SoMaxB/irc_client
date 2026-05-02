#include "DccHandler.hpp"
#include <QFileInfo>
#include <QDir>
#include <QDebug>

DccHandler::DccHandler(QObject* parent)
    : QObject(parent), m_downloadDirectory(QDir::home().filePath("Downloads")),
      m_currentFilesize(0), m_currentPort(0), m_listenPort(0) {
}

DccHandler::~DccHandler() {
    for (DccTransfer* transfer : m_transfers) {
        if (transfer->socket && transfer->socket->isOpen()) {
            transfer->socket->disconnectFromHost();
            transfer->socket->deleteLater();
        }
        if (transfer->server && transfer->server->isListening()) {
            transfer->server->close();
            transfer->server->deleteLater();
        }
        delete transfer;
    }
    m_transfers.clear();
}

void DccHandler::initiateDccSend(const QString& filename, const QString& remoteNick,
                                  const QHostAddress& targetAddress, quint16 targetPort) {
    QFile file(filename);
    if (!file.exists()) {
        qWarning() << "File does not exist:" << filename;
        emit transferFailed(QString(), "File not found: " + filename);
        return;
    }

    QFileInfo fileInfo(file);
    quint32 filesize = static_cast<quint32>(fileInfo.size());

    QString transferId = generateTransferId();

    auto* transfer = new DccTransfer;
    transfer->id = transferId;
    transfer->filename = fileInfo.fileName();
    transfer->filesize = filesize;
    transfer->remoteNick = remoteNick;
    transfer->remoteAddress = targetAddress;
    transfer->remotePort = targetPort;
    transfer->listenPort = 0;
    transfer->bytesTransferred = 0;
    transfer->isIncoming = false;
    transfer->isAccepted = false;
    transfer->isCompleted = false;
    transfer->isCancelled = false;
    transfer->isListening = false;
    transfer->localPath = filename;

    // Listen for incoming connection
    transfer->server = new QTcpServer(this);
    if (!transfer->server->listen(QHostAddress::Any, 0)) {
        qWarning() << "Failed to start DCC server:" << transfer->server->errorString();
        emit transferFailed(transferId, "Cannot listen: " + transfer->server->errorString());
        delete transfer;
        return;
    }

    transfer->listenPort = transfer->server->serverPort();
    transfer->isListening = true;

    QObject::connect(transfer->server, &QTcpServer::newConnection,
                     this, &DccHandler::onNewConnection);

    m_transfers.append(transfer);

    // Note: We need to send the CTCP DCC message to the remote client
    // The MainWindow should send: PRIVMSG nick :\x01DCC SEND filename ip port size\x01
    qDebug() << "DCC listening for" << filename << "on port" << transfer->listenPort;
}

void DccHandler::acceptIncomingTransfer(const QString& transferId) {
    for (DccTransfer* transfer : m_transfers) {
        if (transfer->id == transferId && !transfer->isAccepted && !transfer->isCancelled) {

            m_downloadDirectory = m_downloadDirectory.isEmpty()
                ? QDir::home().filePath("Downloads")
                : m_downloadDirectory;

            QString savePath = QDir(m_downloadDirectory).filePath(transfer->filename);

            transfer->socket = new QTcpSocket(this);
            QObject::connect(transfer->socket, &QTcpSocket::connected, this, [this, transfer]() {
                qDebug() << "DCC connected to sender for incoming transfer" << transfer->id;
            });
            QObject::connect(transfer->socket, &QTcpSocket::readyRead,
                             this, &DccHandler::onSocketReadyRead);
            QObject::connect(transfer->socket, &QTcpSocket::disconnected,
                             this, &DccHandler::onSocketDisconnected);
QObject::connect(transfer->socket, &QTcpSocket::errorOccurred, this, &DccHandler::onSocketError);

            transfer->socket->connectToHost(transfer->remoteAddress, transfer->remotePort);
            transfer->isAccepted = true;
            transfer->localPath = savePath;

            qDebug() << "Accepting DCC transfer:" << transferId
                    << "connecting to" << transfer->remoteAddress.toString()
                    << ":" << transfer->remotePort;
            return;
        }
    }
    qWarning() << "Transfer not found:" << transferId;
}

void DccHandler::rejectIncomingTransfer(const QString& transferId) {
    for (DccTransfer* transfer : m_transfers) {
        if (transfer->id == transferId && !transfer->isAccepted) {
            transfer->isCancelled = true;
            emit transferCancelled(transferId);
            qDebug() << "Rejected DCC transfer:" << transferId;
            return;
        }
    }
}

void DccHandler::cancelTransfer(const QString& transferId) {
    for (DccTransfer* transfer : m_transfers) {
        if (transfer->id == transferId) {
            if (transfer->socket && transfer->socket->isOpen()) {
                transfer->socket->disconnectFromHost();
            }
            if (transfer->server && transfer->server->isListening()) {
                transfer->server->close();
            }
            transfer->isCancelled = true;
            emit transferCancelled(transferId);
            qDebug() << "Cancelled DCC transfer:" << transferId;
            return;
        }
    }
}

QStringList DccHandler::getActiveTransfers() const {
    QStringList result;
    for (DccTransfer* transfer : m_transfers) {
        if (!transfer->isCompleted && !transfer->isCancelled) {
            result.append(transfer->id);
        }
    }
    return result;
}

QString DccHandler::getTransferInfo(const QString& transferId) const {
    for (DccTransfer* transfer : m_transfers) {
        if (transfer->id == transferId) {
            QString direction = transfer->isIncoming ? QChar(0x2193) : QChar(0x2191);
            QString status;

            if (transfer->isCancelled) {
                status = "Cancelled";
            } else if (transfer->isCompleted) {
                status = "Completed";
            } else if (transfer->socket && transfer->socket->state() == QAbstractSocket::ConnectedState) {
                status = "Transferring";
            } else if (transfer->isAccepted) {
                status = "Connecting";
            } else if (transfer->isListening) {
                status = "Waiting";
            } else {
                status = "Pending";
            }

            return QString("%1 %2 - %3: %4 / %5")
                .arg(direction, transfer->filename, status,
                     bytesToHumanReadable(transfer->bytesTransferred),
                     bytesToHumanReadable(transfer->filesize));
        }
    }
    return QString();
}

QString DccHandler::processIncomingDcc(const QString& remoteNick, const QString& filename,
                                        quint32 filesize, const QHostAddress& address, quint16 port) {
    QString transferId = generateTransferId();

    auto* transfer = new DccTransfer;
    transfer->id = transferId;
    transfer->filename = filename;
    transfer->filesize = filesize;
    transfer->remoteNick = remoteNick;
    transfer->remoteAddress = address;
    transfer->remotePort = port;
    transfer->listenPort = 0;
    transfer->bytesTransferred = 0;
    transfer->isIncoming = true;
    transfer->isAccepted = false;
    transfer->isCompleted = false;
    transfer->isCancelled = false;
    transfer->isListening = false;

    m_currentRemoteNick = remoteNick;
    m_currentFilename = filename;
    m_currentFilesize = filesize;
    m_currentAddress = address;
    m_currentPort = port;

    m_downloadDirectory = m_downloadDirectory.isEmpty()
        ? QDir::home().filePath("Downloads")
        : m_downloadDirectory;

    QString savePath = QDir(m_downloadDirectory).filePath(filename);
    transfer->localPath = savePath;

    // Don't auto-connect - wait for user to accept via acceptIncomingTransfer()
    m_transfers.append(transfer);

    emit dccRequestReceived(remoteNick, filename, filesize, transferId);

    qDebug() << "Incoming DCC queued:" << filename << "from" << remoteNick
            << "(" << filesize << "bytes) - waiting for acceptance";

    return transferId;
}

void DccHandler::setDownloadDirectory(const QString& dir) {
    m_downloadDirectory = dir;
}

QString DccHandler::downloadDirectory() const {
    return m_downloadDirectory;
}

quint16 DccHandler::getListenPort() const {
    for (DccTransfer* t : m_transfers) {
        if (t->isListening && t->server && t->server->isListening()) {
            return t->server->serverPort();
        }
    }
    return 0;
}

void DccHandler::removeTransfer(const QString& transferId) {
    for (DccTransfer* transfer : m_transfers) {
        if (transfer->id == transferId) {
            cleanupCompletedTransfer(transfer);
            return;
        }
    }
}

void DccHandler::cleanupAllTransfers() {
    QList<DccTransfer*> toRemove = m_transfers;
    for (DccTransfer* transfer : toRemove) {
        cleanupCompletedTransfer(transfer);
    }
}

void DccHandler::onNewConnection() {
    auto* server = qobject_cast<QTcpServer*>(sender());
    if (!server) return;

    qDebug() << "Incoming DCC connection";
    while (server->hasPendingConnections()) {
        QTcpSocket* socket = server->nextPendingConnection();
        if (!socket) continue;

        qDebug() << "DCC connection from" << socket->peerAddress().toString();

        // Find the transfer waiting for this connection
        for (DccTransfer* transfer : m_transfers) {
            if (transfer->server == server && !transfer->isAccepted && !transfer->isCancelled) {
                acceptConnection(transfer, socket);

                cleanupCompletedTransfer(transfer);
                return;
            }
        }

        socket->disconnectFromHost();
    }
}

void DccHandler::onSocketReadyRead() {
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    for (DccTransfer* transfer : m_transfers) {
        if (transfer->socket == socket && transfer->isIncoming && transfer->isAccepted) {
            processIncomingData(transfer);
            return;
        }
    }
}

void DccHandler::onSocketDisconnected() {
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    for (DccTransfer* transfer : m_transfers) {
        if (transfer->socket == socket) {
            if (!transfer->isCompleted && !transfer->isCancelled) {
                transfer->isCompleted = true;
                emit transferCompleted(transfer->id);
            }
            qDebug() << "DCC socket disconnected for transfer" << transfer->id;
            return;
        }
    }
}

void DccHandler::onSocketError() {
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qWarning() << "DCC socket error:" << socket->errorString();

    for (DccTransfer* transfer : m_transfers) {
        if (transfer->socket == socket) {
            emit transferFailed(transfer->id, socket->errorString());
            return;
        }
    }
}

void DccHandler::acceptConnection(DccTransfer* transfer, QTcpSocket* socket) {
    QObject::connect(socket, &QTcpSocket::readyRead,
                     this, &DccHandler::onSocketReadyRead);
    QObject::connect(socket, &QTcpSocket::disconnected,
                     this, &DccHandler::onSocketDisconnected);
    QObject::connect(socket, &QTcpSocket::errorOccurred,
                     this, &DccHandler::onSocketError);

    transfer->socket = socket;
    transfer->isAccepted = true;

    if (!transfer->isIncoming) {
        QFile file(transfer->localPath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();

            qint64 written = socket->write(data);
            socket->flush();

            transfer->bytesTransferred = written;
            transfer->isCompleted = true;

            qDebug() << "DCC file sent:" << transfer->filename
                    << "(" << written << "bytes)";

            socket->disconnectFromHost();
            socket->deleteLater();
            transfer->socket = nullptr;

            transfer->server->close();
            transfer->server->deleteLater();
            transfer->server = nullptr;
            transfer->isListening = false;

            emit transferCompleted(transfer->id);
        } else {
            emit transferFailed(transfer->id, "Cannot read file");
        }
    }

    qDebug() << "DCC connection accepted for transfer" << transfer->id;
}

void DccHandler::processIncomingData(DccTransfer* transfer) {
    if (!transfer || !transfer->socket) return;

    QByteArray data = transfer->socket->readAll();
    if (data.isEmpty()) return;

    QFile file(transfer->localPath);
    if (!file.isOpen()) {
        if (!file.open(QIODevice::Append | QIODevice::WriteOnly)) {
            emit transferFailed(transfer->id, "Cannot open file for writing: " + file.errorString());
            return;
        }
    }

    qint64 written = file.write(data);
    file.flush();

    transfer->bytesTransferred += written;

    emit transferProgress(transfer->id, transfer->bytesTransferred,
                        transfer->filesize);

    if (transfer->bytesTransferred >= transfer->filesize) {
        file.close();
        transfer->isCompleted = true;

        if (transfer->socket) {
            transfer->socket->disconnectFromHost();
            transfer->socket->deleteLater();
            transfer->socket = nullptr;
        }

        if (transfer->server && transfer->server->isListening()) {
            transfer->server->close();
            transfer->server->deleteLater();
            transfer->server = nullptr;
            transfer->isListening = false;
        }

        emit transferCompleted(transfer->id);
        qDebug() << "DCC transfer completed:" << transfer->id
                << "-" << transfer->filename;
    }
}

QString DccHandler::generateTransferId() {
    static int counter = 0;
    return QString("dcc_%1").arg(++counter);
}

void DccHandler::cleanupCompletedTransfer(DccTransfer* transfer) {
    if (!transfer) return;

    m_transfers.removeAll(transfer);

    if (transfer->socket) {
        if (transfer->socket->isOpen()) {
            transfer->socket->disconnectFromHost();
        }
        transfer->socket->deleteLater();
    }
    if (transfer->server) {
        if (transfer->server->isListening()) {
            transfer->server->close();
        }
        transfer->server->deleteLater();
    }
    delete transfer;
}

QString DccHandler::bytesToHumanReadable(qint64 bytes) const {
    if (bytes < 1024) {
        return QString("%1 B").arg(bytes);
    } else if (bytes < 1024 * 1024) {
        return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    } else if (bytes < 1024 * 1024 * 1024) {
        return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 2);
    } else {
        return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
}