#include "ConfigManager.hpp"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

ConfigManager::ConfigManager() {
    // Initialize QSettings with application organization and name
    // This creates config files in standard locations:
    // Linux: ~/.config/IrcClient/IrcClient.conf
    // Windows: HKEY_CURRENT_USER\Software\IrcClient\IrcClient
    // macOS: ~/Library/Preferences/com.irc.client.plist
    m_settings = std::make_unique<QSettings>("IrcClient", "IrcClient");
}

ConfigManager::~ConfigManager() {
    // Settings are automatically saved on QSettings destruction
}

void ConfigManager::saveLastConnection(const QString& server, int port, const QString& nickname, bool useTls) {
    m_settings->setValue(LAST_SERVER, server);
    m_settings->setValue(LAST_PORT, port);
    m_settings->setValue(LAST_NICKNAME, nickname);
    m_settings->setValue(LAST_USE_TLS, useTls);
    m_settings->sync(); // Ensure immediate write to disk
    qDebug() << "Saved last connection:" << server << ":" << port << "nick:" << nickname << "tls:" << useTls;
}

bool ConfigManager::loadLastConnection(QString& server, int& port, QString& nickname, bool& useTls) {
    if (!m_settings->contains(LAST_SERVER)) {
        qDebug() << "No last connection settings found";
        return false;
    }

    server = m_settings->value(LAST_SERVER, "irc.libera.chat").toString();
    port = m_settings->value(LAST_PORT, 6697).toInt();
    nickname = m_settings->value(LAST_NICKNAME, "User").toString();
    useTls = m_settings->value(LAST_USE_TLS, true).toBool();
    
    qDebug() << "Loaded last connection:" << server << ":" << port << "nick:" << nickname << "tls:" << useTls;
    return true;
}

void ConfigManager::addJoinedChannel(const QString& channel) {
    QStringList channels = m_settings->value(JOINED_CHANNELS, QStringList()).toStringList();
    
    if (!channels.contains(channel)) {
        channels.append(channel);
        m_settings->setValue(JOINED_CHANNELS, channels);
        m_settings->sync();
        qDebug() << "Added channel to history:" << channel;
    }
}

void ConfigManager::removeJoinedChannel(const QString& channel) {
    QStringList channels = m_settings->value(JOINED_CHANNELS, QStringList()).toStringList();
    channels.removeAll(channel);
    m_settings->setValue(JOINED_CHANNELS, channels);
    m_settings->sync();
    qDebug() << "Removed channel from history:" << channel;
}

QStringList ConfigManager::getJoinedChannels() const {
    QStringList channels = m_settings->value(JOINED_CHANNELS, QStringList()).toStringList();
    qDebug() << "Retrieved joined channels:" << channels;
    return channels;
}

void ConfigManager::clearJoinedChannels() {
    m_settings->remove(JOINED_CHANNELS);
    m_settings->sync();
    qDebug() << "Cleared all joined channels";
}

void ConfigManager::saveAutoReconnectEnabled(bool enabled) {
    m_settings->setValue(AUTO_RECONNECT_ENABLED, enabled);
    m_settings->sync();
    qDebug() << "Saved auto-reconnect preference:" << enabled;
}

bool ConfigManager::loadAutoReconnectEnabled() const {
    return m_settings->value(AUTO_RECONNECT_ENABLED, true).toBool();
}

void ConfigManager::saveCustomPreset(const QString& name, const QString& server, int port, bool useTls) {
    QString groupKey = QString("Presets/") + name;
    m_settings->setValue(groupKey + "/Server", server);
    m_settings->setValue(groupKey + "/Port", port);
    m_settings->setValue(groupKey + "/UseTls", useTls);
    m_settings->sync();
    qDebug() << "Saved custom preset:" << name << "-" << server << ":" << port;
}

bool ConfigManager::loadCustomPresets(QStringList& presetNames, QStringList& servers, QList<int>& ports, QList<bool>& tlsFlags) {
    m_settings->beginGroup("Presets");
    presetNames = m_settings->childGroups();
    
    // Filter out built-in groups like "CustomPresets"
    presetNames.removeAll("CustomPresets");
    
    if (presetNames.isEmpty()) {
        m_settings->endGroup();
        qDebug() << "No custom presets found";
        return false;
    }

    servers.clear();
    ports.clear();
    tlsFlags.clear();

    for (const auto& preset : presetNames) {
        servers.append(m_settings->value(preset + "/Server", "").toString());
        ports.append(m_settings->value(preset + "/Port", 6667).toInt());
        tlsFlags.append(m_settings->value(preset + "/UseTls", false).toBool());
    }

    m_settings->endGroup();
    qDebug() << "Loaded" << presetNames.count() << "custom presets";
    return true;
}

void ConfigManager::removeCustomPreset(const QString& name) {
    QString groupKey = QString("Presets/") + name;
    m_settings->remove(groupKey);
    m_settings->sync();
    qDebug() << "Removed custom preset:" << name;
}

void ConfigManager::clearAllSettings() {
    m_settings->clear();
    m_settings->sync();
    qDebug() << "Cleared all settings";
}
