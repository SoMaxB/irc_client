#pragma once

#include <QString>
#include <QStringList>
#include <QSettings>
#include <memory>

/**
 * @class ConfigManager
 * @brief Manages application configuration persistence
 * 
 * Handles loading and saving:
 * - Last used server, port, nickname
 * - Previously joined channels
 * - Custom server presets (future)
 * - User preferences
 */
class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    // Last connection config
    void saveLastConnection(const QString& server, int port, const QString& nickname, bool useTls);
    bool loadLastConnection(QString& server, int& port, QString& nickname, bool& useTls);

    // Channel history
    void addJoinedChannel(const QString& channel);
    void removeJoinedChannel(const QString& channel);
    QStringList getJoinedChannels() const;
    void clearJoinedChannels();

    // User preferences
    void saveAutoReconnectEnabled(bool enabled);
    bool loadAutoReconnectEnabled() const;

    // Theme preference
    void saveThemePreference(int themeIndex);
    int loadThemePreference() const;

    // Server presets (custom ones, beyond defaults)
    void saveCustomPreset(const QString& name, const QString& server, int port, bool useTls);
    bool loadCustomPresets(QStringList& presetNames, QStringList& servers, QList<int>& ports, QList<bool>& tlsFlags);
    void removeCustomPreset(const QString& name);

    // Utility
    void clearAllSettings();

private:
    std::unique_ptr<QSettings> m_settings;
    
    // Settings keys
    static constexpr const char* LAST_SERVER = "Connection/LastServer";
    static constexpr const char* LAST_PORT = "Connection/LastPort";
    static constexpr const char* LAST_NICKNAME = "Connection/LastNickname";
    static constexpr const char* LAST_USE_TLS = "Connection/LastUseTls";
    static constexpr const char* JOINED_CHANNELS = "Channels/JoinedChannels";
    static constexpr const char* AUTO_RECONNECT_ENABLED = "Preferences/AutoReconnectEnabled";
    static constexpr const char* THEME_PREFERENCE = "Preferences/UseAltTheme";
    static constexpr const char* CUSTOM_PRESETS = "Presets/CustomPresets";
};
