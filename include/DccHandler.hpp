#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QVector>
#include <QPointer>

struct DccTransfer {
    QString id;
    QString filename;
    quint32 filesize;
    QString remoteNick;
    QHostAddress remoteAddress;
    quint16 remotePort;
    quint16 listenPort;
    qint64 bytesTransferred;
    qint64 bytesToWrite;
    bool isIncoming;
    bool isAccepted;
    bool isCompleted;
    bool isCancelled;
    bool isListening;
    QPointer<QTcpSocket> socket;
    QPointer<QTcpServer> server;
    QString localPath;
};

class DccHandler : public QObject {
    Q_OBJECT

public:
    explicit DccHandler(QObject* parent = nullptr);
    ~DccHandler();

    void initiateDccSend(const QString& filename, const QString& remoteNick,
                         const QHostAddress& targetAddress, quint16 targetPort);
    void acceptIncomingTransfer(const QString& transferId);
    void rejectIncomingTransfer(const QString& transferId);
    void cancelTransfer(const QString& transferId);
    QStringList getActiveTransfers() const;
    QString getTransferInfo(const QString& transferId) const;
    QString processIncomingDcc(const QString& remoteNick, const QString& filename,
                                quint32 filesize, const QHostAddress& address, quint16 port);
    void setDownloadDirectory(const QString& dir);
    QString downloadDirectory() const;
    quint16 getListenPort() const;
    void removeTransfer(const QString& transferId);
    void cleanupAllTransfers();

signals:
    void dccRequestReceived(const QString& nick, const QString& filename,
                           quint32 filesize, const QString& transferId);
    void transferProgress(const QString& transferId, qint64 bytes, qint64 total);
    void transferCompleted(const QString& transferId);
    void transferFailed(const QString& transferId, const QString& error);
    void transferCancelled(const QString& transferId);

private slots:
    void onNewConnection();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onSocketError();

private:
    void acceptConnection(DccTransfer* transfer, QTcpSocket* socket);
    void processIncomingData(DccTransfer* transfer);
    void onFileSendComplete(DccTransfer* transfer);
    void cleanupCompletedTransfer(DccTransfer* transfer);
    QString bytesToHumanReadable(qint64 bytes) const;
    QString generateTransferId();

    QVector<DccTransfer*> m_transfers;
    QString m_downloadDirectory;
    QString m_currentRemoteNick;
    QString m_currentFilename;
    quint32 m_currentFilesize;
    QHostAddress m_currentAddress;
    quint16 m_currentPort;
    quint16 m_listenPort;
};